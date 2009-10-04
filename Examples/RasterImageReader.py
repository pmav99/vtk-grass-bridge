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
init.Init("RasterImageReader")
init.ExitOnErrorOn()

#List all raster maps
maplister = vtkGRASSRasterMapLister()
name = maplister.GetFileNames().GetValue(0)
print "Open raster map ", name

#Choose the first raster map in the list (which is hopefully not empty)
rs = vtkGRASSRasterImageReader()
rs.SetRasterName(name)
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

#We display the raster image in the first window
#and the processed image in the second window

#Create mouse interaction for the first viewer
viewInt1 = vtkRenderWindowInteractor()
#Create the first image viewer
viewer1 = vtkImageViewer2()
viewer1.SetInputConnection(rs.GetOutputPort())
viewer1.SetColorWindow(255)
viewer1.SetupInteractor(viewInt1)
viewer1.Render()
#Create mouse interaction for the second viewer
viewInt2 = vtkRenderWindowInteractor()
#Create the second image viewer
viewer2 = vtkImageViewer2()
viewer2.SetInputConnection(filter.GetOutputPort())
viewer2.SetColorWindow(255)
viewer2.SetupInteractor(viewInt2)
viewer2.Render()
#Start the interaction
viewInt1.Start()
viewInt2.Start()
