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
from vtk import *
from libvtkGRASSBridgeRasterPython import *
from libvtkGRASSBridgeCommonPython import *

class GRASSRasterMapReaderWriterTest1(unittest.TestCase):
    def setUp(self):
        init = vtkGRASSInit()
        init.Init("GRASSRasterMapReaderWriterTest1")
        init.ExitOnErrorOn()

    def testSimpleReadWriteCycle(self):

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(100)
        region.SetRows(100)
        region.AdjustRegionResolution()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_cell")

        data = vtkIntArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        min = 100000000
        max = 0

        for i in range(writer.GetNumberOfRows()):
            for j in range(writer.GetNumberOfCols()):
                val = i + j + 100
                if min > val:
                    min = val
                if max < val:
                    max = val
                data.SetTuple1(j, val)
            writer.PutNextRow(data)

        print writer
        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_cell")

        newMin = 100000000
        newMax = 0
        for i in range(reader.GetNumberOfRows()):
            data = reader.GetRow(i)
            for j in range(reader.GetNumberOfCols()):
                val =  int(data.GetTuple1(j))
                if newMin > val:
                    newMin = val
                if newMax < val:
                    newMax = val

        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader

        self.assertEqual(val[0], min, "Error range is incorrect")
        self.assertEqual(val[1], max, "Error range is incorrect")
        self.assertEqual(reader.GetNumberOfRows(), region.GetRows(), "Error rows differ")
        self.assertEqual(reader.GetNumberOfCols(), region.GetCols(), "Error cols differ")

        reader.CloseMap()

        # FCELL MAP read and write

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToFCELL()
        writer.UseCurrentRegion()
        writer.OpenMap("test_fcell")

        data = vtkIntArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        min = 100000000
        max = 0

        for i in range(writer.GetNumberOfRows()):
            for j in range(writer.GetNumberOfCols()):
                val = i + j + 100
                if min > val:
                    min = val
                if max < val:
                    max = val
                data.SetTuple1(j, val)
            writer.PutNextRow(data)

        print writer
        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_fcell")

        newMin = 100000000
        newMax = 0
        for i in range(reader.GetNumberOfRows()):
            data = reader.GetRow(i)
            for j in range(reader.GetNumberOfCols()):
                val =  int(data.GetTuple1(j))
                if newMin > val:
                    newMin = val
                if newMax < val:
                    newMax = val

        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader

        self.assertEqual(val[0], min, "Error range is incorrect")
        self.assertEqual(val[1], max, "Error range is incorrect")
        self.assertEqual(reader.GetNumberOfRows(), region.GetRows(), "Error rows differ")
        self.assertEqual(reader.GetNumberOfCols(), region.GetCols(), "Error cols differ")

        reader.CloseMap()


        # DCELL MAP read and write

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToDCELL()
        writer.UseCurrentRegion()
        writer.OpenMap("test_dcell")

        data = vtkIntArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        min = 100000000
        max = 0

        for i in range(writer.GetNumberOfRows()):
            for j in range(writer.GetNumberOfCols()):
                val = i + j + 100
                if min > val:
                    min = val
                if max < val:
                    max = val
                data.SetTuple1(j, val)
            writer.PutNextRow(data)

        print writer
        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_dcell")

        newMin = 100000000
        newMax = 0
        for i in range(reader.GetNumberOfRows()):
            data = reader.GetRow(i)
            for j in range(reader.GetNumberOfCols()):
                val =  int(data.GetTuple1(j))
                if newMin > val:
                    newMin = val
                if newMax < val:
                    newMax = val

        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader

        self.assertEqual(val[0], min, "Error range is incorrect")
        self.assertEqual(val[1], max, "Error range is incorrect")
        self.assertEqual(reader.GetNumberOfRows(), region.GetRows(), "Error rows differ")
        self.assertEqual(reader.GetNumberOfCols(), region.GetCols(), "Error cols differ")

        reader.CloseMap()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRasterMapReaderWriterTest1)
    unittest.TextTestRunner(verbosity=2).run(suite)
