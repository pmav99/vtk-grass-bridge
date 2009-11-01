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
#% key: f
#% description: Filter null values
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
#%Option
#% key: aerror
#% type: double
#% required: no
#% multiple: no
#% key_desc: aerror
#% answer: 0.1
#% description: Specify the absolute error of the mesh; that is, the error in height between the decimated mesh and the original height field. You need to set this value only when the error measure is set to AbsoluteError.
#%End
#%Option
#% key: rerror
#% type: double
#% required: no
#% multiple: no
#% key_desc: aerror
#% answer: 0.1
#% description: Specify the relative error of the mesh; that is, the error in height between the decimated mesh and the original height field normalized by the diagonal of the image. You need to set this value only when the error measure is set to RelativeError.
#%End
#%Option
#% key: reduction
#% type: double
#% required: no
#% multiple: no
#% key_desc: reduction
#% answer: 0.1
#% description: Specify the relative error of the mesh; that is, the error in height between the decimated mesh and the original height field normalized by the diagonal of the image. You need to set this value only when the error measure is set to RelativeError.
#%End
#%Option
#% key: mode
#% type: string
#% required: no
#% multiple: no
#% key_desc: mode
#% options: reduction,trinum,aerror,rerror
#% answer: reduction
#% description: Specify how to terminate the algorithm: either as an absolute number of triangles, a relative number of triangles (normalized by the full resolution mesh), an absolute error (in the height field), or relative error (normalized by the length of the diagonal of the image).
#%End
#%Option
#% key: null
#% type: string
#% required: no
#% multiple: no
#% key_desc: mode
#% answer: -9999
#% description: The value to be used for grass null values in vtk
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
    trinum = int(options['trinum'])
    aerror = float(options['aerror'])
    null = float(options['null'])
    reduction = float(options['reduction'])
    mode = options['mode']
    rerror = float(options['rerror'])
    build_topo = int(flags['t'])
    write_vtk = int(flags['w'])
    show = int(flags['s'])
    thres = int(flags['f'])

    # Initiate GRASS
    init = vtkGRASSInit()

    # Raster map reader
    reader = vtkGRASSRasterImageReader()
    reader.SetRasterName(input)
    reader.UseCurrentRegion()
    #reader.SetNullValue(null);

    # The VTK filter
    filter = vtkGreedyTerrainDecimation()
    filter.SetInputConnection(reader.GetOutputPort())
    filter.BoundaryVertexDeletionOn ()
#    filter.ComputeNormalsOn ()
    if mode == "trinum":
        filter.SetErrorMeasureToNumberOfTriangles ()
    if mode == "reduction":
        filter.SetErrorMeasureToSpecifiedReduction ()
    if mode == "aerror":
        filter.SetErrorMeasureToAbsoluteError ()
    if mode == "rerror":
        filter.SetErrorMeasureToRelativeError ()
#    filter.BoundaryVertexDeletionOff ()
    filter.ComputeNormalsOff ()
    filter.SetReduction (reduction)
    filter.SetAbsoluteError (aerror)
    filter.SetRelativeError (rerror)
    filter.SetNumberOfTriangles (trinum)
    filter.Update()

    if thres == 1:
        print "Start thresholde filter"
        threshold = vtkThreshold()
        threshold.SetInputConnection(filter.GetOutputPort())
        threshold.ThresholdByUpper (null + 2)
        threshold.AllScalarsOff()
        threshold.Update()

        print "Extract polydata"
        geometry = vtkGeometryFilter()
        geometry.SetInputConnection(threshold.GetOutputPort())
        geometry.Update()
        generateVectorOutput(build_topo, output, geometry, write_vtk, show)
    else:
        # Generate the output
        generateVectorOutput(build_topo, output, filter, write_vtk, show)

if __name__ == "__main__":
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("r.to.tin")
    init.ExitOnErrorOn()

    options, flags = grass.parser()
    main()
