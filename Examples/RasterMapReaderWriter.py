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

#The region settings
region = vtkGRASSRegion()
region.ReadCurrentRegion()
region.SetCols(10)
region.SetRows(10)
region.AdjustRegion()

#Create a new map and write some data into it
writer = vtkGRASSRasterMapWriter()
writer.SetMapTypeToCELL()
writer.SetRegion(region)
writer.UseUserDefinedRegion()
writer.OpenMap("test_cell")

#this is the row buffer
data = vtkIntArray()
data.SetNumberOfTuples(writer.GetNumberOfCols())

#iterate over each raster cell
for i in range(writer.GetNumberOfRows()):
    for j in range(writer.GetNumberOfCols()):
        data.SetTuple1(j, i + j + 100)
    writer.PutNextRow(data)

#close the map
writer.CloseMap()

#now reopen the map for reading
reader = vtkGRASSRasterMapReader()
reader.OpenMap("test_cell")

#print the cells to stdout
print " "
for i in range(reader.GetNumberOfRows()):
    data = reader.GetRow(i)
    for j in range(reader.GetNumberOfCols()):
        print data.GetTuple1(j),
    print " "

#print the range of the raster map
val = [0,0]
reader.GetRange(val);
print "Range ", val

#print the history
print reader.GetHistory()
reader.CloseMap()

