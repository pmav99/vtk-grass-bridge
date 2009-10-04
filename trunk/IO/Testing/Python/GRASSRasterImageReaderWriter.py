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
from libvtkGRASSBridgeCommonPython import *

class GRASSRasterSourceTest(unittest.TestCase):
    def testRegion(self):

        init = vtkGRASSInit()
        init.Init("v.GRASSRasterSourceTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("lsat7_2002_80")
        rs.UseCurrentRegion()
	rs.Update()
        print rs.GetOutput()

        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("lsat7_2002_80")
        rs.UseDefaultRegion()
	rs.Update()
        print rs.GetOutput()
        
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("lsat7_2002_80")
        rs.UseRasterRegion()
	rs.Update()
        print rs.GetOutput()
        
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("lsat7_2002_80")
        rs.UseDefaultRegion()
	rs.Update()
        print rs.GetOutput()

    def testReadWrite(self):
        init = vtkGRASSInit()
        init.Init("v.GRASSRasterSourceTest")
        init.ExitOnErrorOn()
        reader = vtkGRASSRasterImageReader()
        reader.SetRasterName("lsat7_2002_80")
        reader.UseRasterRegion()
	reader.Update()
        print reader.GetOutput()

        writer = vtkGRASSRasterImageWriter()
        writer.SetRasterName("test_lsat7")
        writer.SetRegion(reader.GetRegion())
        writer.UseUserDefinedRegion()
        writer.SetInputConnection(reader.GetOutputPort())
	writer.Update()
        print writer.GetOutput()

    def testSmoke(self):
        init = vtkGRASSInit()
        init.Init("v.GRASSRasterSourceTest")
        init.ExitOnErrorOn()
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("lsat7_2002_80")
        rs.UseRasterRegion()
	rs.Update()
        print rs.GetRegion()
        print rs.GetOutput()

        #filter = vtkImageGradient()
        #filter = vtkImageIslandRemoval2D()
        #filter = vtkImageLaplacian()
        #filter = vtkImageMagnitude()
        #filter = vtkImageNormalize()
        #filter = vtkImageSobel2D()
        filter = vtkImageGaussianSmooth()
        filter.SetInputConnection(rs.GetOutputPort())

        viewInt1 = vtkRenderWindowInteractor()

        viewer1 = vtkImageViewer2()
        viewer1.SetInputConnection(filter.GetOutputPort())
        viewer1.SetColorWindow(255)
        viewer1.SetupInteractor(viewInt1)
        viewer1.Render()

        viewInt2 = vtkRenderWindowInteractor()

        viewer2 = vtkImageViewer2()
        viewer2.SetInputConnection(rs.GetOutputPort())
        viewer2.SetColorWindow(255)
        viewer2.SetupInteractor(viewInt2)
        viewer2.Render()

        viewInt1.Start()
        viewInt2.Start()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRasterSourceTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

