#!/usr/bin/env python
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

#include the VTK and vtkGRASSBridge Python libraries
import unittest
import subprocess
from libvtkCommonPython import *
from libvtkFilteringPython import *
from libvtkGraphicsPython import *
from libvtkRenderingPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *

firstCheck = False

class GRASSRaster3dImageReaderWriterTest(unittest.TestCase):
    def setUp(self):
        global firstCheck
        if firstCheck == False:
            #Initiate grass
            init = vtkGRASSInit()
            init.Init("GRASSRaster3dImageReaderWriterTest")
            init.ExitOnErrorOn()
            # Create the input data
            inputlist = ["r3.mapcalc", "--o", "e=test_map_3d = depth()"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()

    def testSmoke(self):

        rs = vtkGRASSRaster3dImageReader()
        rs.SetRaster3dName("test_map_3d")
        rs.UseRasterRegion()

        ws = vtkGRASSRaster3dImageWriter()
        ws.SetInput(rs.GetOutput())
        ws.SetRaster3dName("test_map_3d_2")
        ws.UseCurrentRegion()
        ws.Update()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRaster3dImageReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)