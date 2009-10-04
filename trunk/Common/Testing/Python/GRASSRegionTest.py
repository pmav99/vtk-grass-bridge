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

        ret = region = vtkGRASSRegion()
        if ret != True:
            print region.GetError();
        print region

        ret =region.ReadDefaultRegion()
        if ret != True:
            print region.GetError();
        print region

        ret = region.ReadCurrentRegion()
        if ret != True:
            print region.GetError();
        print region

        print region
        region.SetRows(region.GetRows() - 250);
        region.SetCols(region.GetCols() - 250);
        ret = region.AdjustRegion()
        if ret != True:
            print region.GetError();
        print region

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

