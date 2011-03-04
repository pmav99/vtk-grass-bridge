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
from libvtkGRASSBridgeCommonPython import *

firstCheck = False

class GRASSRasterSourceTest(unittest.TestCase):
    def setUp(self):
        global firstCheck
        if firstCheck == False:
            init = vtkGRASSInit()
            init.Init("GRASSRasterSourceTest")
            init.ExitOnErrorOn()

            inputlist = ["g.region", "-d"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            
            inputlist = ["r.random.cells", "--o", "output=random_cells", "distance=10.0"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True  
            
    def testRegion(self):

        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.UseCurrentRegion()
	rs.Update()
        print rs.GetOutput()

        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.UseDefaultRegion()
	rs.Update()
        print rs.GetOutput()
        
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.UseRasterRegion()
	rs.Update()
        print rs.GetOutput()
        
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.UseDefaultRegion()
	rs.Update()
        print rs.GetOutput()

    def testReadWrite(self):

        reader = vtkGRASSRasterImageReader()
        reader.SetRasterName("random_cells")
        reader.UseRasterRegion()
	reader.Update()
        print reader.GetOutput()

        writer = vtkGRASSRasterImageWriter()
        writer.SetRasterName("test_random_cells")
        writer.SetRegion(reader.GetRegion())
        writer.UseUserDefinedRegion()
        writer.SetInputConnection(reader.GetOutputPort())
	writer.Update()
        print writer.GetOutput()

    def testSmokePoint(self):

        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.UseRasterRegion()
	rs.Update()
        #print rs.GetRegion()
        #print rs.GetOutput()

        viewInt = vtkRenderWindowInteractor()

        viewer = vtkImageViewer()
        viewer.SetInputConnection(rs.GetOutputPort())
        viewer.SetColorWindow(255)
        viewer.SetupInteractor(viewInt)
        viewer.Render()

        viewInt.Start()


    def testSmokeCell(self):

        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.AsCellDataOn()
        rs.UseRasterRegion()
	rs.Update()
        #print rs.GetRegion()
        #print rs.GetOutput()
        
        writer = vtkStructuredPointsWriter()
        writer.SetInputConnection(rs.GetOutputPort())
        writer.SetFileName("/tmp/test.vtk")
        writer.Write()

        viewInt = vtkRenderWindowInteractor()
        viewer = vtkImageViewer()
        viewer.SetInputConnection(rs.GetOutputPort())
        viewer.SetColorWindow(255)
        viewer.SetupInteractor(viewInt)
        viewer.Render()

        viewInt.Start()
        
if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRasterSourceTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

