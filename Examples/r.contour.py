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

    # Initiate GRASS
    init = vtkGRASSInit()

    # Raster map reader
    reader = vtkGRASSRasterImageReader()
    reader.SetRasterName(input)
    reader.UseCurrentRegion()

    # The VTK filter
    filter = vtkContourFilter()
    filter.SetInputConnection(reader.GetOutputPort())
#    filter.SetNumberOfContours(int(contours))
    filter.GenerateValues(int(contours), float(start), float(end))

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
