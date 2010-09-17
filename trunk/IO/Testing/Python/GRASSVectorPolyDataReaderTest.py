#
#  Program: vtkGRASSBridge
#  COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

import unittest
import os
import subprocess
from libvtkCommonPython import *
from libvtkFilteringPython import *
from libvtkGraphicsPython import *
from libvtkRenderingPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeRasterPython import *
from libvtkGRASSBridgeVectorPython import *
from libvtkGRASSBridgeCommonPython import *

firstCheck = False

class GRASSVectorPolyDataReaderTest(unittest.TestCase):

    def setUp(self):
        global firstCheck
        if firstCheck == False:
            # Create the input data
            inputlist = ["v.random", "--o", "column=elev", "n=20", "zmin=-20", "zmax=2500", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "-l", "input=random_points", "output=random_lines"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "input=random_points", "output=random_areas"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

    def view(self, input):
        return
        normals = vtkPolyDataNormals()
        normals.SetInput(input)
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

    def testNoTopoReader(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("random_areas")
        rs.Update()

        self.view(rs.GetOutput())

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/test1.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

    def testTopoReaderAreas(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToArea()
        rs.SetVectorName("random_areas")
        rs.Update()

        self.view(rs.GetOutput())
        
        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/testAreas.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

    def testTopoReaderLines(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToLines()
        rs.SetVectorName("random_lines")
        rs.Update()

        self.view(rs.GetOutput())

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/testLines.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

    def testTopoReaderBoundaries(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToBoundary()
        rs.SetVectorName("random_areas")
        rs.Update()

        self.view(rs.GetOutput())

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/testBoundaries.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

    def testTopoReaderCentroids(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToCentroid()
        rs.SetVectorName("random_areas")
        rs.Update()

        self.view(rs.GetOutput())

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/testCentroids.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

    def testTopoReaderPoints(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToPoints()
        rs.SetVectorName("random_points")
        rs.Update()

        self.view(rs.GetOutput())

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/testPoints.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorPolyDataReaderTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

