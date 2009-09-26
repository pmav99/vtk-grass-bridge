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
from libvtkGRASSBridgeVectorPython import *
from libvtkGRASSBridgeCommonPython import *

class GRASSRasterSourceTest(unittest.TestCase):

    def testSmoke(self):
        init = vtkGRASSInit()
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("elev_lid792_randpts")

	delaunay = vtkDelaunay2D()
	delaunay.SetInputConnection(rs.GetOutputPort())

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/test.vtk")
        writer.SetInputConnection(delaunay.GetOutputPort())
        writer.Write()


if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRasterSourceTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

