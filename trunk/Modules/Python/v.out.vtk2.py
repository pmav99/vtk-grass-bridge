#!/usr/bin/env python
#
############################################################################
#
# MODULE:       v.out.vtk2
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Convert a grass vector map into vtkPolyData and adds the
#               database columns as celldata arrays
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
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *


def main():
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("v.out.vtk2")
    init.ExitOnErrorOn()

    module = vtkGRASSModule()
    module.SetDescription("Convert a grass vector map into vtkPolyData")
    module.AddKeyword("vector")
    module.AddKeyword("export")
    module.AddKeyword("vtk")

    input = vtkGRASSOption()
    input.CreateOption_V_INPUT()

    output = vtkGRASSOption()
    output.CreateOption_F_OUTPUT()
    output.SetDescription("The vtk xml poly data output file")

    feature = vtkGRASSOption()
    feature.CreateOption_V_FEATURE()

    paramter = vtkStringArray()
    for arg in sys.argv:
        paramter.InsertNextValue(str(arg))

    init.Parser(paramter)

    messages = vtkGRASSMessagingInterface()

    messages.Message("Converting vector map")

    reader = vtkGRASSVectorTopoPolyDataReader()
    reader.SetVectorName(input.GetAnswer())

if __name__ == "__main__":
    main()
