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

class GRASSVectorPolyDataReaderTest(unittest.TestCase):

    def testNoTopoReader(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

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
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

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
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

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
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

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
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

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
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/testPoints.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorPolyDataReaderTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

