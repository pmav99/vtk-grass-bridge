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
from vtk import *

# A small selection of available image filter 
# for image processing in VTK
# vtkImageGradient()
# vtkImageIslandRemoval2D()
# vtkImageLaplacian()
# vtkImageMagnitude()
# vtkImageNormalize()
# vtkImageSobel2D()

from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeRasterPython import *
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
#We choose the smooth
filter = vtkImageGaussianSmooth()
filter.SetInputConnection(rs.GetOutputPort())

writer = vtkXMLImageDataWriter()
writer.SetInput(filter.GetOutput())
writer.SetFileName("/tmp/test.vti")
writer.Write()

# Write the generated 3D raster map back to GRASS
ws = vtkGRASSRaster3dImageWriter()
ws.SetInput(filter.GetOutput())
ws.SetRaster3dName("zImageGaussianSmooth")
ws.UseCurrentRegion()
ws.Update()
