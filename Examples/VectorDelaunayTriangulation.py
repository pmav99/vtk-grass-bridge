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

# The purpose of this example is to demonstrate 
# the to read a grass vector map without topology information
# into the VTK poly data format and processing that map with vtkDelaunay2D.
# The processed data is written as VTK XML file to the file system

from libvtkCommonPython import *
from libvtkFilteringPython import *
from libvtkGraphicsPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeVectorPython import *
from libvtkGRASSBridgeCommonPython import *

# Init grass variables
init = vtkGRASSInit()

# Now build the pipeline
# read the vector map without creating topology
reader = vtkGRASSVectorPolyDataReader() # The reader does not need topology information
reader.SetVectorName("elev_lid792_randpts")
# start the delaunay triangulation
delaunay = vtkDelaunay2D()
delaunay.SetInputConnection(reader.GetOutputPort())
# write the data as XML with base64 encoding
writer = vtkXMLPolyDataWriter()
writer.SetFileName("/tmp/test.vtk")
writer.SetInputConnection(delaunay.GetOutputPort())
writer.Write()

