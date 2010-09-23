#!/usr/bin/env python
#
############################################################################
#
# MODULE:       d.rast
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Visualize a raster map using VTK image render window
#
# COPYRIGHT:    (C) 2009 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################


#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkFilteringPython import *
from libvtkRenderingPython import *
from libvtkGraphicsPython import *
from libvtkCommonPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
import sys

def main():
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("d.rast")
    init.ExitOnErrorOn()

    module = vtkGRASSModule()
    module.SetDescription("Visualize a raster map using VTK image render window")
    module.AddKeyword("raster")
    module.AddKeyword("display")

    raster =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputType())

    # Put the command line arguments into a vtk string array and pass it to the parser
    parameter = vtkStringArray()
    for arg in sys.argv:
        parameter.InsertNextValue(str(arg))

    if init.Parser(parameter) != True:
        return -1
    
    # Raster map reader
    reader = vtkGRASSRasterImageReader()
    reader.SetRasterName(raster.GetAnswer())
    reader.UseCurrentRegion()
    reader.SetNullValue(0)
    reader.Update()

    viewInt1 = vtkRenderWindowInteractor()

    viewer1 = vtkImageViewer2()
    viewer1.SetInputConnection(reader.GetOutputPort())
    viewer1.SetupInteractor(viewInt1)
    viewer1.Render()

    viewInt1.Start()

if __name__ == "__main__":
    main()
