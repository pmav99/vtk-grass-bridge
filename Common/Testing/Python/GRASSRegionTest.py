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


class GRASSRegionTest(unittest.TestCase):
    def setUp(self):
        
        init = vtkGRASSInit()
        init.Init("GRASSRegionTest")
        init.ExitOnErrorOn()

    def testSmoke(self):

        region = vtkGRASSRegion()
        print "Current region", region

        ret = region.ReadDefaultRegion()
        if ret != True:
            print region.GetError();
        print "Default region", region

        ret = region.ReadCurrentRegion()
        if ret != True:
            print region.GetError();
        print "Current region", region

        region.SetRows(region.GetRows() + 25);
        region.SetCols(region.GetCols() + 25);
        ret = region.AdjustRegionResolution()
        if ret != True:
            print region.GetError();
        print "Modified region rows and cols", region

        region.SetRows3d(region.GetRows3d() + 25);
        region.SetCols3d(region.GetCols3d() + 25);
        region.SetDepths(region.GetCols3d() + 25);
        ret = region.AdjustRegion3dResolution()
        if ret != True:
            print region.GetError();
        print "Modified region 3d", region

        region.SetEastWestResolution(region.GetEastWestResolution() + 25);
        region.SetNorthSouthResolution(region.GetNorthSouthResolution() + 25);
        ret = region.AdjustRegionFromResolution()
        if ret != True:
            print region.GetError();
        print "Modified region resolution", region

        region.SetEastWestResolution3d(region.GetEastWestResolution3d() + 25);
        region.SetNorthSouthResolution3d(region.GetNorthSouthResolution3d() + 25);
        region.SetTopBottomResolution(region.GetTopBottomResolution() + 25);
        region.SetTop(region.GetTop() + 25);
        region.SetBottom(region.GetBottom() + 25);
        ret = region.AdjustRegion3dFromResolution()
        if ret != True:
            print region.GetError();
        print "Modified region 3d resolution", region

        ret = region.SetCurrentRegion()
        if ret != True:
            print region.GetError();

        ret = region.ReadCurrentRegion()
        if ret != True:
            print region.GetError();
        print "Current region", region

        print "Save regions"
        ret = region.SaveRegion("testregion")
        if ret != True:
            print region.GetError();
        ret = region.SaveRegion("testregion_b")
        if ret != True:
            print region.GetError();

        lister = vtkGRASSFileLister()
        lister.SetElementToRegion()
        lister.Refresh()
        num = lister.GetFileNames().GetNumberOfValues()
        print "Regions in mapset " + lister.GetMapsetName()
        for i in range(num):
		print lister.GetFileNames().GetValue(i)

    def testRegionSize2d(self):

        region = vtkGRASSRegion()

        ret = region.ReadDefaultRegion()
        if ret != True:
            print region.GetError();
            
        rows = 1000
        cols = 600
        n = region.GetNorthernEdge() + region.GetNorthernEdge()/1000.0
        s = region.GetSouthernEdge() + region.GetNorthernEdge()/1000.0
        e = region.GetEasternEdge() + region.GetEasternEdge()/1000.0
        w = region.GetWesternEdge() + region.GetEasternEdge()/1000.0

        region.SetRows(rows);
        region.SetCols(cols);
        region.SetNorthernEdge(n)
        region.SetSouthernEdge(s)
        region.SetEasternEdge(e)
        region.SetWesternEdge(w)
        region.AdjustRegionResolution()

        ret = region.SetCurrentRegion()
        if ret != True:
            print region.GetError();

        ret = region.ReadCurrentRegion()
        if ret != True:
            print region.GetError();
        print "region", region

        self.assertEqual(region.GetRows(), rows, "Rows differ")
        self.assertEqual(region.GetCols(), cols, "Cols differ")
        self.assertEqual(region.GetNorthernEdge(), n, "north differ")
        self.assertEqual(region.GetSouthernEdge(), s, "south differ")
        self.assertEqual(region.GetEasternEdge(), e, "east differ")
        self.assertEqual(region.GetWesternEdge(), w, "west differ")

    def testRegionSize3d(self):

        region = vtkGRASSRegion()

        ret = region.ReadDefaultRegion()
        if ret != True:
            print region.GetError();

        rows = 1000
        cols = 600
        depths = 100
        n = region.GetNorthernEdge() + region.GetNorthernEdge()/1000.0
        s = region.GetSouthernEdge() + region.GetNorthernEdge()/1000.0
        e = region.GetEasternEdge() + region.GetEasternEdge()/1000.0
        w = region.GetWesternEdge() + region.GetEasternEdge()/1000.0

        region.SetRows3d(rows);
        region.SetCols3d(cols);
        region.SetDepths(depths)
        region.SetNorthernEdge(n)
        region.SetSouthernEdge(s)
        region.SetEasternEdge(e)
        region.SetWesternEdge(w)
        region.AdjustRegion3dResolution()

        ret = region.SetCurrentRegion()
        if ret != True:
            print region.GetError();

        ret = region.ReadCurrentRegion()
        if ret != True:
            print region.GetError();
        print "region", region

        self.assertEqual(region.GetRows3d(), rows, "Rows3d differ")
        self.assertEqual(region.GetCols3d(), cols, "Cols3d differ")
        self.assertEqual(region.GetDepths(), depths, "Depths differ")
        self.assertEqual(region.GetNorthernEdge(), n, "north differ")
        self.assertEqual(region.GetSouthernEdge(), s, "south differ")
        self.assertEqual(region.GetEasternEdge(), e, "east differ")
        self.assertEqual(region.GetWesternEdge(), w, "west differ")

    def testRegionResolution2d(self):

        region = vtkGRASSRegion()

        ret = region.ReadDefaultRegion()
        if ret != True:
            print region.GetError();

        ew_res = 50
        ns_res = 25

        region.SetEastWestResolution(ew_res);
        region.SetNorthSouthResolution(ns_res);
        region.AdjustRegionFromResolution()

        print region

        ret = region.SetCurrentRegion()
        if ret != True:
            print region.GetError();

        ret = region.ReadCurrentRegion()
        if ret != True:
            print region.GetError();
        print "region", region

        self.assertEqual(int(region.GetEastWestResolution()), ew_res, "ew resolution differ")
        self.assertEqual(int(region.GetNorthSouthResolution()), ns_res, "ns resolution differ")

    def testRegionResolution3d(self):

        region = vtkGRASSRegion()

        ret = region.ReadDefaultRegion()
        if ret != True:
            print region.GetError();

        ew_res = 50
        ns_res = 25
        tb_res = 10

        region.SetEastWestResolution3d(ew_res);
        region.SetNorthSouthResolution3d(ns_res);
        region.SetTopBottomResolution(tb_res)
        region.AdjustRegion3dFromResolution()

        print region

        ret = region.SetCurrentRegion()
        if ret != True:
            print region.GetError();

        ret = region.ReadCurrentRegion()
        if ret != True:
            print region.GetError();
        print "region", region

        self.assertEqual(int(region.GetEastWestResolution3d()), ew_res, "ew resolution differ")
        self.assertEqual(int(region.GetNorthSouthResolution3d()), ns_res, "ns resolution differ")
        self.assertEqual(int(region.GetTopBottomResolution()), tb_res, "tb resolution differ")

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRegionTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

