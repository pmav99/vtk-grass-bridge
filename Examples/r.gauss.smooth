#!/usr/bin/env python
#
############################################################################
#
# MODULE:       r.gauss.smooth
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Demonstrate the usage of VTK and vtkGRASSBridge to implement 
#               grass modules
#
# COPYRIGHT:    (C) 2009 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#%Module
#% description: Use gaussian smooth algorithm on raster map
#% keywords: raster
#% keywords: buffer
#%End
#%Option
#% key: input
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name of input raster map
#% gisprompt: old,cell,raster
#%End
#%Option
#% key: output
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name for output raster map
#% gisprompt: new,cell,raster
#%End

#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkFilteringPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
import grass.script as grass


def main():
    input = options['input']
    output = options['output']

    # Initiate GRASS
    init = vtkGRASSInit()

    # Raster map reader
    reader = vtkGRASSRasterImageReader()
    reader.SetRasterName(input)
    reader.UseCurrentRegion()

    # The VTK filter
    filter = vtkImageGaussianSmooth()
    filter.SetInputConnection(reader.GetOutputPort())

    # Write the result  
    writer = vtkGRASSRasterImageWriter()
    writer.SetInputConnection(filter.GetOutputPort())
    writer.UseCurrentRegion()
    writer.SetRasterName(output)
    writer.Update()


if __name__ == "__main__":
    options, flags = grass.parser()
    main()
