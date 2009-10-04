#!/usr/bin/env python
#
############################################################################
#
# MODULE:       v.delaunay2d
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Delaunay 2d triangulation using the vtkDelaunay2D class.
#               The created vtk triangles will be converted into the grass
#               vector feature face.
#
# COPYRIGHT:    (C) 2009 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#%Module
#% description: Delaunay 2d triangulation using the vtkDelaunay2D class. The created triangles will be converted into the grass vector feature face.
#% keywords: vector
#% keywords: delaunay
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
#% description: Name of input vector map
#% gisprompt: old,vect,vector
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
#% key: alpha
#% type: double
#% required: no
#% multiple: no
#% key_desc: alpha
#% answer: 0
#% description: Specify alpha (or distance) value to control output of this filter. For a non-zero alpha value, only edges or triangles contained within a sphere centered at mesh vertices will be output. Otherwise, only triangles will be output.
#%End
#%Option
#% key: tolerance
#% type: double
#% required: no
#% multiple: no
#% key_desc: tolerance
#% answer: 0
#% description: Specify a tolerance to control discarding of closely spaced points. This tolerance is specified as a fraction of the diagonal length of the bounding box of the points.
#%End

#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkFilteringPython import *
from libvtkRenderingPython import *
from libvtkGraphicsPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
import grass.script as grass


def main():
    input = options['input']
    output = options['output']
    alpha = options['alpha']
    tolerance = options['tolerance']
    build_topo = int(flags['t'])
    write_vtk = int(flags['w'])
    show = int(flags['s'])

    # Initiate GRASS

    init = vtkGRASSInit()
    init.Init("v.delaunay2d")
    init.ExitOnErrorOn()


    # Now build the pipeline
    # read the vector map without creating topology
    reader = vtkGRASSVectorPolyDataReader() 
    reader.SetVectorName(input)

    # start the delaunay triangulation
    delaunay = vtkDelaunay2D()
    delaunay.SetInputConnection(reader.GetOutputPort())
    delaunay.SetAlpha(float(alpha))
    delaunay.SetTolerance(float(tolerance))

    # write the data grass vector map
    writer = vtkGRASSVectorPolyDataWriter()
    writer.SetVectorName(output)
    writer.SetInputConnection(delaunay.GetOutputPort())
    if build_topo == 1:
        writer.BuildTopoOn()
    else:
        writer.BuildTopoOff()
    writer.Update()

    # write optionally the vtk data as XML file for visualisation purposes
    if write_vtk == 1:
        xmlwriter = vtkXMLPolyDataWriter()
        xmlwriter.SetFileName(output + ".vtk")
        xmlwriter.SetInputConnection(delaunay.GetOutputPort())
        xmlwriter.Write()
    
    if show == 1:
        normals = vtkPolyDataNormals()
        normals.SetInputConnection(delaunay.GetOutputPort())
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
