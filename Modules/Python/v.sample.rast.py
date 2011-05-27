#!/usr/bin/env python
#
############################################################################
#
# MODULE:       v.sample.rast
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Sample a raster map based on the points of an input vector map
#
# COPYRIGHT:    (C) 2010 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#include the grass, VTK and vtkGRASSBridge Python libraries
from vtk import *

from libvtkGRASSBridgeVectorPython import *
from libvtkGRASSBridgeRasterPython import *
from libvtkGRASSBridgeDbmiPython import *
from libvtkGRASSBridgeCommonPython import *
import sys

def main():
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("v.sample.rast")
    init.ExitOnErrorOn()

    module = vtkGRASSModule()
    module.SetDescription("Sample a raster map based on the points of an input vector map")
    module.AddKeyword("vector")
    module.AddKeyword("sample")

    input =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorInputType())
    output =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorOutputType())
    raster =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputType())
    raster.SetKey("raster")
    
    column = vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetDataBaseColumnType())
    column.SetDescription("Name of the column added to the output vector map")
    column.SetDefaultAnswer("rast_val")
        
    # Put the command line arguments into a vtk string array and pass it to the parser
    parameter = vtkStringArray()
    for arg in sys.argv:
        parameter.InsertNextValue(str(arg))

    if init.Parser(parameter) != True:
        return -1

    # Use the GRASS GIS messaging interface for gm and noisy output
    gm = vtkGRASSMessagingInterface()
    
    # Open the raster map
    rastermap = vtkGRASSRasterMapReader()
    rastermap.OpenMap(raster.GetAnswer())
    
    # Open the input vector map
    vectormap = vtkGRASSVectorMapTopoReader()
    vectormap.OpenMap(input.GetAnswer())

    # Open the output vector map
    outputmap = vtkGRASSVectorMapWriter()
    outputmap.OpenMap(output.GetAnswer())
    
    # Setup the new SQL table for the output map
    db = outputmap.GetDbmiInterface()
    table = vtkGRASSDbmiTable()
    column1 = vtkGRASSDbmiColumn()
    column2 = vtkGRASSDbmiColumn()

    column1.SetName("cat")
    column1.SetSQLTypeToInteger()

    column2.SetName(column.GetAnswer())
    column2.SetSQLTypeToDoublePrecision()

    table.SetName(output.GetAnswer())
    table.AppendColumn(column1)
    table.AppendColumn(column2)

    # Create the table
    db.ConnectDBCreateTable(table)

    db.GetTable(table)

    points = vtkGRASSVectorFeaturePoints()
    cats = vtkGRASSVectorFeatureCats()
    
    # Start the transaction
    db.BeginTransaction()
    
    cat = 1
    for i in range(vectormap.GetNumberOfFeatures()):
        gm.Percent(i, vectormap.GetNumberOfFeatures(), 1)
        # Break at the end of the vector file
        if vectormap.ReadFeature(i + 1, points, cats) < 0:
            break
        # Read only points and add the value to the new vector map
        if points.IsFeatureTypePoint():
            val = vtk.mutable(1)
            p = points.GetPoint(0)
            # Sample the raster value
            if rastermap.GetNearestSampleValue(p[0], p[1], val):
                
                # Create a new vector category for each found sample
                newcats = vtkGRASSVectorFeatureCats()
                newcats.AddCat(1, cat)
                
                # Append the point and the new category to the output map
                outputmap.WriteFeature(points, newcats)
                
                # SQL statement to insert the found value into the output vector table
                string = "INSERT INTO " + str(table.GetName()) + " values (" +\
                          str(cat) + "," + str(val) + ")"
                          
                db.ExecuteImmediate(string)
                
                # Next category
                cat += 1

        
    # End transaction
    db.CommitTransaction()
        
    gm.Percent(1, 1, 1)
    
    db.DisconnectDB()
        
    vectormap.CloseMap()
    rastermap.CloseMap()
    outputmap.CloseMap()

if __name__ == "__main__":
    exit(main())
