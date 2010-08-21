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
    def testSmoke(self):

        init = vtkGRASSInit()
        init.Init("GRASSRegionTest")
        init.ExitOnErrorOn()

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
        ret = region.AdjustRegion()
        if ret != True:
            print region.GetError();
        print "Modified region rows and cols", region

        region.SetRows3d(region.GetRows3d() + 25);
        region.SetCols3d(region.GetCols3d() + 25);
        region.SetDepths(region.GetCols3d() + 25);
        ret = region.AdjustRegion3d()
        if ret != True:
            print region.GetError();
        print "Modified region 3d", region

        region.SetEastWestResolution(region.GetEastWestResolution() + 25);
        region.SetNorthSouthResolution(region.GetNorthSouthResolution() + 25);
        ret = region.AdjustRegion()
        if ret != True:
            print region.GetError();
        print "Modified region resolution", region

        region.SetEastWestResolution3d(region.GetEastWestResolution3d() + 25);
        region.SetNorthSouthResolution3d(region.GetNorthSouthResolution3d() + 25);
        region.SetTopBottomResolution(region.GetTopBottomResolution() + 25);
        region.SetTop(region.GetTop() + 25);
        region.SetBottom(region.GetBottom() + 25);
        ret = region.AdjustRegion3d()
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

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRegionTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

