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
from vtk import *
from libvtkGRASSBridgeCommonPython import *
from libvtkGRASSBridgeVectorPython import *

class GRASSVectorLinePointsTest(unittest.TestCase):
    def setUp(self):

        #Initiate grass
        init = vtkGRASSInit()
        init.Init("GRASSVectorLinePointsTest")
        init.ExitOnErrorOn()
        self.points = vtkGRASSVectorFeaturePoints()
        self.points.AppendPoint(0.0, 0.0, 0.0)
        self.points.AppendPoint(1.0, 0.0, 0.0)
        self.points.AppendPoint(2.0, 0.0, 0.0)
        self.points.AppendPoint(3.0, 0.0, 0.0)
        self.points.AppendPoint(4.0, 0.0, 0.0)
        self.points.AppendPoint(5.0, 0.0, 0.0)


    def test2Simple(self):
        self.assertEqual(self.points.GetNumberOfPoints(), 6, "Number of points is incorrect")

        self.points.SetPoint(0, 0.0, 1.0, 0.0)

        print self.points
        val = self.points.GetPoint(0)

        self.assertAlmostEqual(val[0], 0.0, 4, "xValue is incorrect")
        self.assertAlmostEqual(val[1], 1.0, 4, "yValue is incorrect")
        self.assertAlmostEqual(val[2], 0.0, 4, "zValue is incorrect")

        self.points.DeletePoint(0)

        print self.points
        self.assertEqual(self.points.GetNumberOfPoints(), 5, "Number of points is incorrect after removing")

        length = self.points.GetLineLength()
        self.assertAlmostEqual(length, 4, 4, "length is incorrect")

        self.points.InsertPoint(4, 0.5, 0.5, 0.5)
        val = self.points.GetPoint(4)

        self.assertAlmostEqual(val[0], 0.5, 4, "xValue is incorrect")
        self.assertAlmostEqual(val[1], 0.5, 4, "yValue is incorrect")
        self.assertAlmostEqual(val[2], 0.5, 4, "zValue is incorrect")

        print self.points
        self.assertEqual(self.points.GetNumberOfPoints(), 6, "Number of points is incorrect after insert")

    def test1Segment(self):
        segment = vtkGRASSVectorFeaturePoints()
        start = 1
        end = self.points.GetNumberOfPoints() - 1
        self.points.CreateLineSegment(start, end, segment)

        print segment

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorLinePointsTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
