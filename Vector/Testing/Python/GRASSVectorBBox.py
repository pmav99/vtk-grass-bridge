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
from libvtkGRASSBridgeCommonPython import *
from libvtkGRASSBridgeVectorPython import *

class GRASSVectorBBoxTest(unittest.TestCase):
    def setUp(self):

        #Initiate grass
        init = vtkGRASSInit()
        self.box = vtkGRASSVectorBBox()
        self.box.SetBox(100, 50, 200, 150, 200, 0)

    def testFull(self):

        ret = self.box.PointInBox(170, 70, 100)
        self.assertEqual(ret, 1, "PointInBox failure")

        newbox = vtkGRASSVectorBBox()
        newbox.SetBox(200, 70, 300, 170, 300, 100)

        ret = self.box.Overlap(newbox)
        self.assertEqual(ret, 1, "Overlap failure")

        newbox.Extent(self.box)
        self.assertEqual(newbox.GetSouth(), self.box.GetSouth(), "Extent south failure")
        self.assertEqual(newbox.GetWest(), self.box.GetWest(), "Extent west failure")
        self.assertEqual(newbox.GetBottom(), self.box.GetBottom(), "Extent bottom failure")

        newbox.DeepCopy(self.box)
        self.assertEqual(newbox.GetNorth(), self.box.GetNorth(), "Extent north failure")
        self.assertEqual(newbox.GetEast(), self.box.GetEast(), "Extent east failure")
        self.assertEqual(newbox.GetTop(), self.box.GetTop(), "Extent top failure")

        region = vtkGRASSRegion()
        newbox.CopyBoxFromRegion(region)
        self.assertEqual(newbox.GetSouth(), region.GetSouthernEdge(), "Copy region south failure")
        self.assertEqual(newbox.GetWest(), region.GetWesternEdge(), "Copy region west failure")
        self.assertEqual(newbox.GetNorth(), region.GetNorthernEdge(), "Copy region north failure")
        self.assertEqual(newbox.GetEast(), region.GetEasternEdge(), "Copy region east failure")

#        print newbox
#        print region
#        print self.box
  

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorBBoxTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
