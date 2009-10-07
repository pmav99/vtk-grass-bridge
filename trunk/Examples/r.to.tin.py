#!/usr/bin/env python
#
############################################################################
#
# MODULE:       r.to.tin
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
#    filter.SetErrorMeasureToSpecifiedReduction ()
#    filter.SetErrorMeasureToAbsoluteError ()
#    filter.SetErrorMeasureToRelativeError ()
    filter.SetNumberOfTriangles (int(trinum))
    filter.BoundaryVertexDeletionOn ()
#    filter.BoundaryVertexDeletionOff ()
    filter.ComputeNormalsOn ()
#    filter.ComputeNormalsOff ()
#    filter.SetReduction (1)
#    filter.SetAbsoluteError (0.00001)
#    filter.SetRelativeError (0.00001)

    # write the data grass vector map
    writer = vtkGRASSVectorPolyDataWriter()
    writer.SetVectorName(output)
    writer.SetInputConnection(filter.GetOutputPort())
    if build_topo == 1:
        writer.BuildTopoOn()
    else:
        writer.BuildTopoOff()
    writer.Update()

    # write optionally the vtk data as XML file for visualisation purposes
    if write_vtk == 1:
        xmlwriter = vtkXMLPolyDataWriter()
        xmlwriter.SetFileName(output + ".vtk")
        xmlwriter.SetInputConnection(filter.GetOutputPort())
        xmlwriter.Write()

    if show == 1:
        normals = vtkPolyDataNormals()
        normals.SetInputConnection(filter.GetOutputPort())
        mapMesh = vtkPolyDataMapper()
        mapMesh.SetInputConnection(normals.GetOutputPort())
        meshActor = vtkActor()
        meshActor.SetMapper(mapMesh)

        ren = vtkRenderer()
        renWin =vtkRenderWindow()
        renWin.AddRenderer(ren)
        iren = vtkRenderWindowInteractor()
        iren.SetRenderWindow(renWin)

        ren.AddActor(meshActor)
        ren.SetBackground(1, 1, 1)
        renWin.SetSize(800, 600)

        iren.Initialize()
        renWin.Render()
        iren.Start()


if __name__ == "__main__":
    options, flags = grass.parser()
    main()
