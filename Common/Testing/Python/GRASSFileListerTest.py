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

"""Tests for the raster file lister"""
class vtkGRASSRasterMapListerTest(unittest.TestCase):
    def testSmoke(self):

        init = vtkGRASSInit()
        init.Init("vtkGRASSRasterMapListerTest")
        init.ExitOnErrorOn()

        rml = vtkGRASSRasterMapLister()
        num = rml.GetFileNames().GetNumberOfValues()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)

	rml.SetMapsetName("PERMANENT")
	rml.Refresh()
        num = rml.GetFileNames().GetNumberOfValues()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)


"""Tests for the vector file lister"""
class vtkGRASSVectorMapListerTest(unittest.TestCase):
    def testSmoke(self):

        init = vtkGRASSInit()
        init.Init("vtkGRASSRasterMapListerTest")
        init.ExitOnErrorOn()
        rml = vtkGRASSVectorMapLister()
        num = rml.GetFileNames().GetNumberOfValues()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)

	rml.SetMapsetName("PERMANENT")
	rml.Refresh()
        num = rml.GetFileNames().GetNumberOfValues()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)


"""Tests for the general file lister"""
class vtkGRASSFileListerTest(unittest.TestCase):
    def testSmoke(self):

        init = vtkGRASSInit()
        init.Init("vtkGRASSRasterMapListerTest")
        init.ExitOnErrorOn()
        rml = vtkGRASSFileLister()
	rml.SetMapsetName("PERMANENT")
        rml.SetElementToRegion()
	rml.Refresh()
        num = rml.GetFileNames().GetNumberOfValues()
        print "Region files in mapset " + rml.GetMapsetName()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)

        rml.SetElementToGroup()
	rml.Refresh()
        num = rml.GetFileNames().GetNumberOfValues()
        print "Groups in mapset " + rml.GetMapsetName()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)

        rml.SetElementToRegion3d()
	rml.Refresh()
        num = rml.GetFileNames().GetNumberOfValues()
        print "Region3d files in mapset " + rml.GetMapsetName()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)


        rml.SetElementToGrid3()
	rml.Refresh()
        num = rml.GetFileNames().GetNumberOfValues()
        print "Grid3 files in mapset " + rml.GetMapsetName()
        for i in range(num):
		print rml.GetFileNames().GetValue(i)

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(vtkGRASSRasterMapListerTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
    suite = unittest.TestLoader().loadTestsFromTestCase(vtkGRASSVectorMapListerTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
    suite = unittest.TestLoader().loadTestsFromTestCase(vtkGRASSFileListerTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

