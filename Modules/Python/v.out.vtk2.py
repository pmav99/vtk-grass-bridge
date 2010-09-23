#!/usr/bin/env python
#
############################################################################
#
# MODULE:       v.out.vtk2
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Writes features of a grass vector map as vtk poly data xml files
#               and adds the selectable database columns as cell data arrays
#
# COPYRIGHT:    (C) 2010 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkCommonPython import *
from libvtkFilteringPython import *
from libvtkRenderingPython import *
from libvtkGraphicsPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *

from vtkGRASSBridgeModuleBase import *
import sys

def main():
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("v.out.vtk2")
    init.ExitOnErrorOn()

    module = vtkGRASSModule()
    module.SetDescription("Writes a grass vector map as vtk poly data xml files")
    module.AddKeyword("vector")
    module.AddKeyword("export")
    module.AddKeyword("vtk")

    input =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorInputType())

    output = vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetFileOutputType(),\
             "output", "The vtk xml poly data output basis file name. The file name will be extended with the"\
             " exported feature name and the prefix .vtp, i.e. (_line.vtp)")

    columns = vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetDataBaseColumnsType())

    field = vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorFieldType())
    
    feature = vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorFeatureType())
    feature.SetDefaultOptions("point,line,boundary,centroid,area,face")
    feature.SetDefaultAnswer("line,area")

    visflag = vtkGRASSFlag()
    visflag.SetKey("s")
    visflag.SetDescription("Enable immediate VTK visualization of exported data in a VTK render window")

    colflag = vtkGRASSFlag()
    colflag.SetKey("n")
    colflag.SetDescription("Do not read any SQL database columns")

    nodata = vtkGRASSOption()
    nodata.SetKey("nodata")
    nodata.SetDefaultAnswer("-999999")
    nodata.RequiredOff()
    nodata.MultipleOff()
    nodata.SetTypeToInteger()
    nodata.SetDescription("Integer value which should be used to identify data "\
                          "without database table entries or valid categories")

    # Put the command line arguments into a vtk string array and pass it to the parser
    parameter = vtkStringArray()
    for arg in sys.argv:
        parameter.InsertNextValue(str(arg))

    if init.Parser(parameter) != True:
        return -1

    # Use the GRASS GIS messaging interface for gm and noisy output
    gm = vtkGRASSMessagingInterface()

    # Names of columns to extract
    colarray = vtkStringArray()
    #Extract the columns
    if columns.GetAnswer():
        for name in columns.GetAnswer().split(","):
            colarray.InsertNextValue(name)

    # Set up the vector reader
    reader = vtkGRASSVectorTopoPolyDataReader()
    reader.SetVectorName(input.GetAnswer())
    reader.SetColumnNames(colarray)
    reader.SetLayer(int(field.GetAnswer()))
    reader.SetNoDataValue(int(nodata.GetAnswer()));
    if colflag.GetAnswer():
        reader.ReadDBTableOff()
    
    # Set up the poly data xml writer and connect it with the reader
    writer = vtkXMLPolyDataWriter()
    writer.SetInput(reader.GetOutput())

    # supported formats: point,line,boundary,centroid,area,face
    for name in feature.GetAnswer().split(","):
        if name == "point":
            gm.Message("Writing feature point")
            reader.SetFeatureTypeToPoint()
            writer.SetFileName(output.GetAnswer() + "_point.vtp")
            writer.Write()
            if visflag.GetAnswer():
                visualizeData(reader)
        if name == "line":
            gm.Message("Writing feature line")
            reader.SetFeatureTypeToLine()
            writer.SetFileName(output.GetAnswer() + "_line.vtp")
            writer.Write()
            if visflag.GetAnswer():
                visualizeData(reader)
        if name == "boundary":
            gm.Message("Writing feature boundary")
            reader.SetFeatureTypeToBoundary()
            writer.SetFileName(output.GetAnswer() + "_boundary.vtp")
            writer.Write()
            if visflag.GetAnswer():
                visualizeData(reader)
        if name == "centroid":
            gm.Message("Writing feature centroid")
            reader.SetFeatureTypeToCentroid()
            writer.SetFileName(output.GetAnswer() + "_centroid.vtp")
            writer.Write()
            if visflag.GetAnswer():
                visualizeData(reader)
        if name == "area":
            gm.Message("Writing feature area")
            reader.SetFeatureTypeToArea()
            writer.SetFileName(output.GetAnswer() + "_area.vtp")
            writer.Write()
            if visflag.GetAnswer():
                visualizeData(reader)
        if name == "face":
            gm.Message("Writing feature face")
            reader.SetFeatureTypeToFace()
            writer.SetFileName(output.GetAnswer() + "_face.vtp")
            writer.Write()
            if visflag.GetAnswer():
                visualizeData(reader)

if __name__ == "__main__":
    exit(main())
