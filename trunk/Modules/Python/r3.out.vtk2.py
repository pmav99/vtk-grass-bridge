#!/usr/bin/env python
#
############################################################################
#
# MODULE:       r3.out.vtk2
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

#include the VTK and vtkGRASSBridge Python libraries

from vtk import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
import sys

def main():
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("r3.out.vtk2")
    init.ExitOnErrorOn()

    module = vtkGRASSModule()
    module.SetDescription("Export a grass raster 3d map as vtk xml image data file")
    module.AddKeyword("vector")
    module.AddKeyword("export")
    module.AddKeyword("vtk")

    input =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRaster3dInputType())

    output = vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetFileOutputType(),\
             "output", "The vtk xml image output file name. Should end with .vti")

    null = vtkGRASSOption()
    null.SetKey("null")
    null.SetDefaultAnswer("-9999")
    null.RequiredOff()
    null.MultipleOff()
    null.SetTypeToInteger()
    null.SetDescription("Integer value which should be used to identify no data (null) voxel")

    # Put the command line arguments into a vtk string array and pass it to the parser
    parameter = vtkStringArray()
    for arg in sys.argv:
        parameter.InsertNextValue(str(arg))

    if init.Parser(parameter) != True:
        return -1

    # Use the GRASS GIS messaging interface for noisy output
    gm = vtkGRASSMessagingInterface()

    gm.Message("Reading voxel data into memory")

    # Set up the vector reader
    reader = vtkGRASSRaster3dImageReader()
    reader.SetRaster3dName(input.GetAnswer())
    reader.SetNullValue(int(null.GetAnswer()))
    reader.Update()

    gm.Message("Writing voxel data as VTK XML image data")

    # Set up the poly data xml writer and connect it with the reader
    writer = vtkXMLImageDataWriter()
    writer.SetInput(reader.GetOutput())
    writer.SetFileName(output.GetAnswer())
    writer.Write()

if __name__ == "__main__":
    exit(main())
