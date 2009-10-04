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

class GRASSVectorPolyDataReaderWriterTest(unittest.TestCase):

    def testNoTopoReaderWriter(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_boundary_county")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOff();
        writer.Update()

    def testTopoReaderWriterAreas(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToArea()
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_boundary_county_Areas")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterLines(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToLines()
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_boundary_county_Lines")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterBoundaries(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToBoundary()
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_boundary_county_Boundaries")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterCentroids(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToCentroid()
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_boundary_county_Centroid")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterPoints(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToPoints()
        rs.SetVectorName("boundary_county@user1")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_boundary_county_Points")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorPolyDataReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

