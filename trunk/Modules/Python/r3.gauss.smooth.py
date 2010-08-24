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
#%FLag
#% key: s
#% description: Show the input and output map in a vtk window after computation
#%End
#%Option
#% key: input
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name of input raster map
#% gisprompt: old,grid3,3d-raster
#%End
#%Option
#% key: output
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name for output raster map
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

    if show == 1:
        viewInt1 = vtkRenderWindowInteractor()

        viewer1 = vtkImageViewer2()
        viewer1.SetInputConnection(filter.GetOutputPort())
        viewer1.SetColorWindow(255)
        viewer1.SetupInteractor(viewInt1)
        viewer1.Render()

        viewInt2 = vtkRenderWindowInteractor()

        viewer2 = vtkImageViewer2()
        viewer2.SetInputConnection(reader.GetOutputPort())
        viewer2.SetColorWindow(255)
        viewer2.SetupInteractor(viewInt2)
        viewer2.Render()

        viewInt1.Start()
        viewInt2.Start()

if __name__ == "__main__":
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("r3.gauss.smooth")
    init.ExitOnErrorOn()

    options, flags = grass.parser()
    main()
