#!/usr/bin/env python
#
############################################################################
#
# MODULE:       vtkGRASSBridgeModuleBase
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Base functions for vtk-grass-bridge modules
#
# COPYRIGHT:    (C) 2009 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkFilteringPython import *
from libvtkRenderingPython import *
from libvtkGraphicsPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
import grass.script as grass


def generateVectorOutput(build_topo, output, filter, write_vtk, show):

    gm = vtkGRASSMessagingInterface()
    gm.Message("write vector into grass database")
    # write the data grass vector map
    writer = vtkGRASSVectorPolyDataWriter()
    writer.SetVectorName(output)
    writer.SetInputConnection(filter.GetOutputPort())
    if build_topo == 1:
        gm.Message("build topology")
        writer.BuildTopoOn()
    else:
        writer.BuildTopoOff()
    writer.Update()

    # write optionally the vtk data as XML file for visualisation purposes
    if write_vtk == 1:
        gm.Message("write VTK file")
        xmlwriter = vtkXMLPolyDataWriter()
        xmlwriter.SetFileName(output + ".vtk")
        xmlwriter.SetInputConnection(filter.GetOutputPort())
        xmlwriter.Write()
    
    if show == 1:
        gm.Message("show visulization")
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

