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
from libvtkGRASSBridgeRaster3dPython import *
from libvtkGRASSBridgeCommonPython import *

class GRASSRaster3dMapReaderWriterTest(unittest.TestCase):
    def setUp(self):
        init = vtkGRASSInit()
        init.Init("GRASSRaster3dMapReaderWriterTest")
        init.ExitOnErrorOn()

    def testSimpleReadWriteCycleDCELL1(self):

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols3d(8)
        region.SetRows3d(13)
        region.SetDepths(7)
        region.SetTop(100)
        region.SetBottom(0)
        region.AdjustRegion3dResolution()

        writer = vtkGRASSRaster3dMapWriter()
        writer.SetMapTypeToDCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_dcell1")

        min = 100000000.0
        max = 0.0

        for k in range(writer.GetNumberOfDepths()):
            for i in range(writer.GetNumberOfRows()):
                for j in range(writer.GetNumberOfCols()):
                    val = k + i + j + 100.5
                    if min > val:
                        min = val
                    if max < val:
                        max = val
                    writer.PutValue(j, i, k, val)

        writer.GetHistory().SetTitle("Testmap")
        print writer
        writer.CloseMap()

        reader = vtkGRASSRaster3dMapReader()
        reader.UseRasterRegion()
        reader.OpenMap("test_dcell1")
        print reader

        newMin = 100000000.0
        newMax = 0.0

        for k in range(reader.GetNumberOfDepths()):
            for i in range(reader.GetNumberOfRows()):
                for j in range(reader.GetNumberOfCols()):
                    val = reader.GetValue(j, i, k)
                    if newMin > val:
                        newMin = val
                    if newMax < val:
                        newMax = val

        print newMin, min, newMax, max
        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        reader.CloseMap()

    def testSimpleReadWriteCycleDCELL2(self):

        writer = vtkGRASSRaster3dMapWriter()
        writer.SetMapTypeToDCELL()
        writer.UseDefaultRegion()
        writer.OpenMap("test_dcell2")

        min = 100000000.0
        max = 0.0

        for k in range(writer.GetNumberOfDepths()):
            for i in range(writer.GetNumberOfRows()):
                for j in range(writer.GetNumberOfCols()):
                    val = k + i + j + 100.75
                    if min > val:
                        min = val
                    if max < val:
                        max = val
                    writer.PutValue(j, i, k, val)

        writer.GetHistory().SetTitle("Testmap")
        print writer
        writer.CloseMap()

        reader = vtkGRASSRaster3dMapReader()
        reader.UseRasterRegion()
        reader.OpenMap("test_dcell2")
        print reader

        newMin = 100000000.0
        newMax = 0.0

        for k in range(reader.GetNumberOfDepths()):
            for i in range(reader.GetNumberOfRows()):
                for j in range(reader.GetNumberOfCols()):
                    val = reader.GetValue(j, i, k)
                    if newMin > val:
                        newMin = val
                    if newMax < val:
                        newMax = val

        print newMin, min, newMax, max
        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        reader.CloseMap()


    def testSimpleReadWriteCycleDCELL3(self):

        region = vtkGRASSRegion()
        region.ReadDefaultRegion()
        region.SetCols3d(100)
        region.SetRows3d(120)
        region.SetDepths(20)
        region.SetTop(100)
        region.SetBottom(0)
        region.AdjustRegion3dResolution()
        region.SetCurrentRegion()

        writer = vtkGRASSRaster3dMapWriter()
        writer.SetMapTypeToDCELL()
        writer.UseCurrentRegion()
        writer.OpenMap("test_dcell3")

        min = 100000000.0
        max = 0.0

        for k in range(writer.GetNumberOfDepths()):
            for i in range(writer.GetNumberOfRows()):
                for j in range(writer.GetNumberOfCols()):
                    val = k + i + j + 100.25
                    if min > val:
                        min = val
                    if max < val:
                        max = val
                    writer.PutValue(j, i, k, val)

        writer.GetHistory().SetTitle("Testmap")
        print writer
        writer.CloseMap()

        reader = vtkGRASSRaster3dMapReader()
        reader.UseRasterRegion()
        reader.OpenMap("test_dcell3")
        print reader

        newMin = 100000000.0
        newMax = 0.0

        for k in range(reader.GetNumberOfDepths()):
            for i in range(reader.GetNumberOfRows()):
                for j in range(reader.GetNumberOfCols()):
                    val = reader.GetValue(j, i, k)
                    if newMin > val:
                        newMin = val
                    if newMax < val:
                        newMax = val

        print newMin, min, newMax, max
        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        reader.CloseMap()

    def testSimpleReadWriteCycleFCELL1(self):

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols3d(8)
        region.SetRows3d(13)
        region.SetDepths(7)
        region.SetTop(100)
        region.SetBottom(0)
        region.AdjustRegion3dResolution()

        writer = vtkGRASSRaster3dMapWriter()
        writer.SetMapTypeToFCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_fcell1")

        min = 100000000.0
        max = 0.0

        for k in range(writer.GetNumberOfDepths()):
            for i in range(writer.GetNumberOfRows()):
                for j in range(writer.GetNumberOfCols()):
                    val = k + i + j + 100.5
                    if min > val:
                        min = val
                    if max < val:
                        max = val
                    writer.PutValue(j, i, k, val)

        writer.GetHistory().SetTitle("Testmap")
        print writer
        writer.CloseMap()

        reader = vtkGRASSRaster3dMapReader()
        reader.UseRasterRegion()
        reader.OpenMap("test_fcell1")
        print reader

        newMin = 100000000.0
        newMax = 0.0

        for k in range(reader.GetNumberOfDepths()):
            for i in range(reader.GetNumberOfRows()):
                for j in range(reader.GetNumberOfCols()):
                    val = reader.GetValue(j, i, k)
                    if newMin > val:
                        newMin = val
                    if newMax < val:
                        newMax = val

        print newMin, min, newMax, max
        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        reader.CloseMap()


if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRaster3dMapReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
