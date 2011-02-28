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
from vtk import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeRasterPython import *
from libvtkGRASSBridgeVectorPython import *
from libvtkGRASSBridgeCommonPython import *

firstCheck = False

class GRASSVectorPolyDataReaderWriterTest(unittest.TestCase):

    def setUp(self):
        global firstCheck
        if firstCheck == False:
            # Create the input data
            inputlist = ["v.random", "--o", "column=elev", "n=10", "zmin=-20", "zmax=2500", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "-l", "input=random_points", "output=random_lines"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "input=random_points", "output=random_areas"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

    def testNoTopoReaderWriter(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("random_areas")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_random_areas_no_topo")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOff();
        writer.Update()

    def testTopoReaderWriter(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("random_areas")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_random_areas_topo")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn();
        writer.Update()

    def testTopoAreaReaderWriter(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataAreaReaderWriterTest")
        init.ExitOnErrorOn()
        # Areas
        b = vtkGRASSVectorTopoPolyDataReader()
        b.SetVectorName("random_areas")
        b.SetFeatureTypeToBoundary()
        b.Update()
        # Centroids
        c = vtkGRASSVectorTopoPolyDataReader()
        c.SetVectorName("random_areas")
        c.SetFeatureTypeToCentroid()
        c.Update()
        
        writer = vtkGRASSVectorPolyDataAreaWriter()
        writer.SetVectorName("test_random_areas_topo_2")
        writer.SetInputConnection(0, b.GetOutputPort())
        writer.SetInputConnection(1, c.GetOutputPort())
        writer.BuildTopoOn();
        writer.Update()
        
    def testTopoReaderWriterAreas(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToArea()
        rs.SetVectorName("random_areas")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_random_areas_Areas")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterLines(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToLine()
        rs.SetVectorName("random_lines")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_random_lines_Lines")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterBoundaries(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToBoundary()
        rs.SetVectorName("random_lines")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_random_lines_Boundaries")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterCentroids(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToCentroid()
        rs.SetVectorName("random_areas")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_random_areas_Centroid")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

    def testTopoReaderWriterPoints(self):
        init = vtkGRASSInit()
        init.Init("GRASSVectorPolyDataReaderWriterTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSVectorTopoPolyDataReader()
        rs.SetFeatureTypeToPoint()
        rs.SetVectorName("random_points")
        rs.Update()

        writer = vtkGRASSVectorPolyDataWriter()
        writer.SetVectorName("test_random_points")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.BuildTopoOn()
        writer.Update()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorPolyDataReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

