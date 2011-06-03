#!/usr/bin/env python
#
############################################################################
#
# MODULE:       v.sample.rast
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Sample raster maps based on the points of an input vector map
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
    module.SetDescription("Sample raster maps based on the points of an input vector map")
    module.AddKeyword("vector")
    module.AddKeyword("sample")

    input =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorInputType())
    output =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorOutputType())
    raster =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputsType())
    raster.SetKey("raster")
    
    # Put the command line arguments into a vtk string array and pass it to the parser
    parameter = vtkStringArray()
    for arg in sys.argv:
        parameter.InsertNextValue(str(arg))

    if init.Parser(parameter) != True:
        return -1

    # Use the GRASS GIS messaging interface for noisy output
    gm = vtkGRASSMessagingInterface()
    
    # Read the raster names
    rasterNames = vtkStringArray()
    raster.GetAnswers(rasterNames)
    
    # Open the input vector map
    vectormap = vtkGRASSVectorMapTopoReader()
    vectormap.OpenMap(input.GetAnswer())

    # Open the output vector map
    outputmap = vtkGRASSVectorMapWriter()
    outputmap.OpenMap(output.GetAnswer())
    
    # Setup the new SQL table for the output map
    db = outputmap.GetDbmiInterface()
    table = vtkGRASSDbmiTable()
    column_cat = vtkGRASSDbmiColumn()

    column_cat.SetName("cat")
    column_cat.SetSQLTypeToInteger()

    # Attach the category column
    table.SetName(output.GetAnswer())
    table.AppendColumn(column_cat)

    # Attach a new column for each raster map use the raster map name as column name
    for map in range(rasterNames.GetNumberOfValues()):
        name = rasterNames.GetValue(map)
        column_rast = vtkGRASSDbmiColumn()
        # Assure that the raster map names are SQL compatible
        column_rast.SetName(name.replace('.', '_'))
        column_rast.SetSQLTypeToDoublePrecision()
        column_rast.SetNullAllowed()
        column_rast.SetDescription("Sampled raster values")
        # Add the column
        table.AppendColumn(column_rast)

    # Create the table
    db.ConnectDBCreateTable(table)

    db.GetTable(table)

    # Create the categories
    db.BeginTransaction();
    for i in range(vectormap.GetNumberOfFeatures()): 
        
        string = "INSERT INTO " + str(table.GetName()) + " (cat) values (" + str(i + 1) + ")"
        # print string
        db.ExecuteImmediate(string);
        
    # End transaction
    db.CommitTransaction();

    points = vtkGRASSVectorFeaturePoints()
    cats = vtkGRASSVectorFeatureCats()
    val = vtkDCELL()
    # Create a new vector category for each found sample
    newcats = vtkGRASSVectorFeatureCats()
    
    # Sample the data for each raster map
    for map in range(rasterNames.GetNumberOfValues()):
        name = rasterNames.GetValue(map)
        
        gm.Message("Sample raster map" + name)
        
        # Open the raster map
        rastermap = vtkGRASSRasterMapReader()
        rastermap.OpenMap(name)
        # The column name
        sql_name = name.replace(".", "_")
    
        # Start the transaction
        db.BeginTransaction()

        for i in range(vectormap.GetNumberOfFeatures()):
            gm.Percent(i, vectormap.GetNumberOfFeatures(), 1)
            # Break at the end of the vector file
            if vectormap.ReadFeature(i + 1, points, cats) < 0:
                break
            # Read only points and add the value to the new vector map
            if points.IsFeatureTypePoint():
                north = points.GetPoint(0)[1]
                east = points.GetPoint(0)[0]
                cat = i + 1

                newcats.Reset()
                newcats.AddCat(1, cat)

                # Append the point and the new category to the output map
                outputmap.WriteFeature(points, newcats)
                
                # Sample the raster value
                if rastermap.GetNearestSampleValue(north, east, val):
                    # SQL statement to insert the found value into the output vector table
                    string = "UPDATE " + str(table.GetName()) + " SET " + sql_name + " = " \
                             + str(val.GetValueAsDouble()) + " WHERE cat = " + str(cat)
                    # print string
                else:
                    # In case nothing found the null value is inserted
                    string = "UPDATE " + str(table.GetName()) + " SET " + sql_name + " = " \
                             + "null" + " WHERE cat = " + str(cat)
                    
                db.ExecuteImmediate(string)

        # End transaction
        db.CommitTransaction()
        # Close the arster map
        rastermap.CloseMap()

    gm.Percent(1, 1, 1)
    
    db.DisconnectDB()
        
    vectormap.CloseMap()
    outputmap.CloseMap()

if __name__ == "__main__":
    exit(main())
