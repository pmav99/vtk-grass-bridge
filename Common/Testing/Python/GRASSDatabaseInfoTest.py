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

""" Basisklasse fuer die Filtering Tests"""
import unittest
import os
from vtk import *
from libvtkGRASSBridgeCommonPython import *


class GRASSDatabaseInfoTest(unittest.TestCase):
    def testSmoke(self):

        init = vtkGRASSInit()
        init.Init("GRASSDatabaseInfoTest")
        init.ExitOnErrorOn()

        dbinfo = vtkGRASSDatabaseInfo()
        print dbinfo

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSDatabaseInfoTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

