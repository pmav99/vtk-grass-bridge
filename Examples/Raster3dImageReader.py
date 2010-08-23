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
from libvtkCommonPython import *
from libvtkFilteringPython import *
from libvtkGraphicsPython import *
from libvtkRenderingPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *

#Initiate grass
init = vtkGRASSInit()
init.Init("Raster3dImageReader")
init.ExitOnErrorOn()

#List all raster maps
maplister = vtkGRASSRaster3dMapLister()
name = maplister.GetFileNames().GetValue(0)
print "Open 3d raster map ", name

#Choose the first raster map in the list (which is hopefully not empty)
rs = vtkGRASSRaster3dImageReader()
rs.SetRaster3dName(name)
#Use the region of the raster map
rs.UseRasterRegion()
rs.Update()
#Available image filter ofr image processing in VTK
#filter = vtkImageGradient()
#filter = vtkImageIslandRemoval2D()
#filter = vtkImageLaplacian()
#filter = vtkImageMagnitude()
#filter = vtkImageNormalize()
#filter = vtkImageSobel2D()
#We choose the smooth
filter = vtkImageGaussianSmooth()
filter.SetInputConnection(rs.GetOutputPort())

writer = vtkXMLImageDataWriter()
writer.SetInput(filter.GetOutput())
writer.SetFileName("/tmp/test.vti")
writer.Write()
