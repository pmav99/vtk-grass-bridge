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

class GRASSRasterMapReaderWriterTest(unittest.TestCase):
    def testSimpleReadWriteCycleDCELL(self):
	init = vtkGRASSInit()

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(10)
        region.SetRows(10)
        region.AdjustRegion()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_dcell")

        data = vtkDoubleArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        for i in range(writer.GetNumberOfRows()):
            for j in range(writer.GetNumberOfCols()):
                data.SetTuple1(j, i + j + 100)
            writer.PutNextRow(data)

        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_dcell")

        print " "
        for i in range(reader.GetNumberOfRows()):
            data = reader.GetRow(i)
            for j in range(reader.GetNumberOfCols()):
                print data.GetTuple1(j),
            print " "

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader.GetHistory()
        reader.CloseMap()

    def testSimpleReadWriteCycleFCELL(self):
	init = vtkGRASSInit()

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(10)
        region.SetRows(10)
        region.AdjustRegion()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_fcell")

        data = vtkDoubleArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        for i in range(writer.GetNumberOfRows()):
            for j in range(writer.GetNumberOfCols()):
                data.SetTuple1(j, i + j + 100)
            writer.PutNextRow(data)

        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_fcell")

        print " "
        for i in range(reader.GetNumberOfRows()):
            data = reader.GetRow(i)
            for j in range(reader.GetNumberOfCols()):
                print data.GetTuple1(j),
            print " "

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader.GetHistory()
        reader.CloseMap()

    def testSimpleReadWriteCycleCELL(self):
	init = vtkGRASSInit()

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(10)
        region.SetRows(10)
        region.AdjustRegion()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_cell")

        data = vtkDoubleArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        for i in range(writer.GetNumberOfRows()):
            for j in range(writer.GetNumberOfCols()):
                data.SetTuple1(j, i + j + 100)
            writer.PutNextRow(data)

        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_cell")

        print " "
        for i in range(reader.GetNumberOfRows()):
            data = reader.GetRow(i)
            for j in range(reader.GetNumberOfCols()):
                print data.GetTuple1(j),
            print " "

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader.GetHistory()
        reader.CloseMap()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRasterMapReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
