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
from libvtkGRASSBridgeRasterPython import *
from libvtkGRASSBridgeVectorPython import *
from libvtkGRASSBridgeCommonPython import *

firstCheck = False

class GRASSMultiVectorPolyDataLineReaderTest(unittest.TestCase):

    def setUp(self):
        global firstCheck
        if firstCheck == False:
            # Create the input data
            inputlist = ["v.random", "--o", "column=elev", "n=700", "zmin=0", 
                         "zmax=100", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "input=random_points", 
                         "output=random_areas"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

        num = 5
            
        self.maps = vtkStringArray()
        self.maps.SetNumberOfTuples(num)
        self.lineLengths = vtkDoubleArray()
        self.lineLengths.SetNumberOfTuples(num)
        
        self.columns = vtkStringArray()
        self.columns.SetNumberOfTuples(1)
        self.alterColumns = vtkStringArray()
        self.alterColumns.SetNumberOfTuples(1)
            
        
    def test1Points(self):
        init = vtkGRASSInit()
        init.Init("GRASSMultiVectorPolyDataLineReaderTest")
        init.ExitOnErrorOn()
        
            
        for i in range(5):
            self.maps.SetValue(i, "random_points")
            self.lineLengths.SetValue(i, 3*(i + 1))
    
        self.columns.SetValue(0, "elev")
        self.alterColumns.SetValue(0, "Elevation")
            
        rs = vtkGRASSMultiVectorPolyDataLineReader()
        rs.SetVectorNames(self.maps)
        rs.SetColumnNames(self.columns)
        rs.SetNewArrayNames(self.alterColumns)
        rs.SetLineLengths(self.lineLengths)
        rs.SetFeatureTypeToPoint()
        rs.Update()

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/GRASSMultiVectorPolyDataLineReaderTest1Points.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()

    def test2Centroids(self):
        init = vtkGRASSInit()
        init.Init("GRASSMultiVectorPolyDataLineReaderTest")
        init.ExitOnErrorOn()
        
            
        for i in range(5):
            self.maps.SetValue(i, "random_areas")
            self.lineLengths.SetValue(i, 3*(i + 1))
    
        self.columns.SetValue(0, "elev")
        self.alterColumns.SetValue(0, "Elevation")
            
        rs = vtkGRASSMultiVectorPolyDataLineReader()
        rs.SetVectorNames(self.maps)
        rs.SetColumnNames(self.columns)
        rs.SetNewArrayNames(self.alterColumns)
        rs.SetLineLengths(self.lineLengths)
        rs.SetFeatureTypeToCentroid()
        rs.Update()

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/GRASSMultiVectorPolyDataLineReaderTest2Points.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()
        
if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSMultiVectorPolyDataLineReaderTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

