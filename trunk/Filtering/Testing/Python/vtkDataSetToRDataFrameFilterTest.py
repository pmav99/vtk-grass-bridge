#!/usr/bin/env python
#
#  Program: vtkGRASSBridge
#  COPYRIGHT: (C) 2011 by Soeren Gebbert, soerengebbert@googlemail.com
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
import unittest
from vtk import *
from libvtkGRASSBridgeFilteringPython import *
from libvtkGRASSBridgeCommonPython import *

class vtkDataSetToRdataFrameTest(unittest.TestCase):
    
    def setUp(self):
        
        # Set up the temporal dataset
        self.tds = vtkTemporalDataSet()
        
        data = vtkDoubleArray()
        data.SetNumberOfTuples(25)
        data.SetName("data")
        data.FillComponent(0,3)

        self.ds1 = vtkPolyData()
        self.ds1.Allocate(5,5)
        self.ds1.GetPointData().SetScalars(data)
        
        self.ds2 = vtkPolyData()
        self.ds2.Allocate(5,5)
        self.ds2.GetPointData().SetScalars(data)

        self.points = vtkPoints()
        
        count = 0
        for i in range(5):
            for j in range(5):
                self.points.InsertNextPoint(i, j, 0)

        self.ds1.SetPoints(self.points)    
        self.ds2.SetPoints(self.points)     
        
    def test1(self):
        
        riface = vtkRInterface()
        for i in range(self.ds1.GetPointData().GetNumberOfArrays()):
            array = self.ds1.GetPointData().GetArray(i)
            riface.AssignVTKDataArrayToRVariable(array, array.GetName())
            print "Added variable " + array.GetName() + " to R"
            
        riface.EvalRscript("ls()", True)
        riface.EvalRscript("x <- c(1,2,3,4,5,6,7,8)", True)
        
    def test2(self):
        
        riface = vtkRInterface()
        ia = riface.AssignRVariableToVTKDataArray("x")
        print ia         
                        
    def test3(self):
        
        riface = vtkRInterface()
        riface.EvalRscript("x = runif(100, -0.5, 0.5)", True)
        ia = riface.AssignRVariableToVTKDataArray("x")
        print ia    
  
        
if __name__ == '__main__':
    suite1 = unittest.TestLoader().loadTestsFromTestCase(vtkDataSetToRdataFrameTest)
    unittest.TextTestRunner(verbosity=2).run(suite1) 
