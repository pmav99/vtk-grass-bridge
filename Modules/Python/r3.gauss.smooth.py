#!/usr/bin/env python
#
############################################################################
#
# MODULE:       r3.gauss.smooth
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Demonstrate the usage of VTK and vtkGRASSBridge to implement
#               grass modules for voxel processing
#
# COPYRIGHT:    (C) 2010 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

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
#% gisprompt: old,grid3,3d-raster
#%End
#%Option
#% key: output
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name for output 3d raster map
#% gisprompt: new,grid3,3d-raster
#%End

#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkFilteringPython import *
from libvtkRenderingPython import *
from libvtkGraphicsPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
import grass.script as grass


def main():
    input = options['input']
    output = options['output']
    show = int(flags['s'])

    # 3d raster map reader
    reader = vtkGRASSRaster3dImageReader()
    reader.SetRaster3dName(input)
    reader.UseCurrentRegion()

    # The VTK filter
    filter = vtkImageGaussianSmooth()
    filter.SetInputConnection(reader.GetOutputPort())

    # Write the result
    writer = vtkGRASSRaster3dImageWriter()
    writer.SetInputConnection(filter.GetOutputPort())
    writer.UseCurrentRegion()
    writer.SetRaster3dName(output)
    writer.Update()

if __name__ == "__main__":
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("r3.gauss.smooth")
    init.ExitOnErrorOn()

    options, flags = grass.parser()
    main()
