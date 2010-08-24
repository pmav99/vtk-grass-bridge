#!/usr/bin/env python
#
############################################################################
#
# MODULE:       r.contour
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Create a contour vector map based on raster map values.
#
# COPYRIGHT:    (C) 2009 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#%Module
#% description: Create a contour vector map based on raster map values.
#% keywords: raster
#% keywords: vector
#% keywords: tin
#%End
#%Flag
#% key: t
#% description: Build vector topology
#%End
#%Flag
#% key: s
#% description: Show the output map in a vtk window after computation
#%End
#%Flag
#% key: w
#% description: Write the triangulated map as vtk xml file to the current working directoy named <output>.vtk
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
#% description: Name for output vector map
#% gisprompt: new,vect,vector
#%End
#%Option
#% key: contours
#% type: integer
#% required: no
#% multiple: no
#% key_desc: contours
#% answer: 20
#% description: Set the number of Contours
#%End
#%Option
#% key: start
#% type: integer
#% required: no
#% multiple: no
#% key_desc: start
#% answer: 0
#% description: Set the start value for contour generation
#%End
#%Option
#% key: end
#% type: integer
#% required: no
#% multiple: no
#% key_desc: end
#% answer: 1000
#% description: Set the end value for contour generation
#%End


#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkFilteringPython import *
from libvtkGraphicsPython import *
from libvtkHybridPython import *
from libvtkImagingPython import *
from libvtkIOPython import *
from libvtkRenderingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
from vtkGRASSBridgeModuleBase import *
import grass.script as grass


def main():
    input = options['input']
    output = options['output']
    contours = options['contours']
    start = options['start']
    end = options['end']
    build_topo = int(flags['t'])
    write_vtk = int(flags['w'])
    show = int(flags['s'])

    # Raster map reader
    reader = vtkGRASSRasterImageReader()
    reader.SetRasterName(input)
    reader.UseCurrentRegion()

    # The VTK contour filter
    filter = vtkContourFilter()
    filter.SetInputConnection(reader.GetOutputPort())
    filter.GenerateValues(int(contours), float(start), float(end))

    # Generate the output
    generateVectorOutput(build_topo, output, filter, write_vtk, show)

if __name__ == "__main__":
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("r.to.tin")
    init.ExitOnErrorOn()

    options, flags = grass.parser()
    main()
