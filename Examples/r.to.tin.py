#!/usr/bin/env python
#
############################################################################
#
# MODULE:       r.to.tin
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Create a triangulated irregular network TIN based on
#               an elevation raster map.
#
# COPYRIGHT:    (C) 2009 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#%Module
#% description: Convert a hight raster map into a vector map
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
#% description: Show the input and output map in a vtk window after computation
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
#% key: trinum
#% type: integer
#% required: no
#% multiple: no
#% key_desc: trinum
#% answer: 20000
#% description: Specify the number of triangles to produce on output. (It is a good idea to make sure this is less than a tessellated mesh at full resolution.) You need to set this value only when the error measure is set to NumberOfTriangles.
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
    trinum = options['trinum']
    build_topo = int(flags['t'])
    write_vtk = int(flags['w'])
    show = int(flags['s'])

    # Initiate GRASS
    init = vtkGRASSInit()

    # Raster map reader
    reader = vtkGRASSRasterImageReader()
    reader.SetRasterName(input)
    reader.UseCurrentRegion()

    # The VTK filter
    filter = vtkGreedyTerrainDecimation()
    filter.SetInputConnection(reader.GetOutputPort())
    filter.SetErrorMeasureToNumberOfTriangles ()
    filter.SetNumberOfTriangles (int(trinum))
    filter.BoundaryVertexDeletionOn ()
    filter.ComputeNormalsOn ()
#    filter.SetErrorMeasureToSpecifiedReduction ()
#    filter.SetErrorMeasureToAbsoluteError ()
#    filter.SetErrorMeasureToRelativeError ()
#    filter.BoundaryVertexDeletionOff ()
#    filter.ComputeNormalsOff ()
#    filter.SetReduction (1)
#    filter.SetAbsoluteError (0.00001)
#    filter.SetRelativeError (0.00001)

    # Generate the output
    generateVectorOutput(build_topo, output, filter, write_vtk, show)

if __name__ == "__main__":
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("r.to.tin")
    init.ExitOnErrorOn()

    options, flags = grass.parser()
    main()
