#!/usr/bin/env python
#
#  Program: vtkGRASSBridge
#  COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#include the VTK and vtkGRASSBridge Python libraries
from libvtkCommonPython import *
from libvtkFilteringPython import *
from libvtkGraphicsPython import *
from libvtkRenderingPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *

import grass.script as grass

#%Module
#% description: Use gaussian smooth algorithm on raster 3d map
#% keywords: raster3d
#% keywords: smooth
#%End
#%Option
#% key: input
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name of input 3d raster map
#% gisprompt: old,g3d,raster3d
#%End
#%Option
#% key: output
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name of output 3d raster map
#% gisprompt: old,g3d,raster3d
#%End

def main():
    input = options['input']
    output = options['output']

    #Choose the first raster map in the list (which is hopefully not empty)
    rs = vtkGRASSRaster3dImageReader()
    rs.SetRaster3dName(input)
    rs.UseCurrentRegion()
    rs.Update()

    filter = vtkImageGaussianSmooth()
    filter.SetInputConnection(rs.GetOutputPort())

    writer = vtkXMLImageDataWriter()
    writer.SetInput(filter.GetOutput())
    writer.SetFileName("/tmp/test.vti")
    writer.Write()

    #Choose the first raster map in the list (which is hopefully not empty)
    ws = vtkGRASSRaster3dImageWriter()
    ws.SetInput(filter.GetOutput())
    ws.SetRaster3dName(output)
    #Use the region of the raster map
    ws.UseCurrentRegion()
    ws.Update()

if __name__ == "__main__":
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("r3.gauss.smooth")
    init.ExitOnErrorOn()

    options, flags = grass.parser()
    main()