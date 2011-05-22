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
from libvtkGRASSBridgeTemporalPython import *
from libvtkGRASSBridgeCommonPython import *

class vtkTemporalDataSetSourceTest(unittest.TestCase):
    
    def setUp(self):
        
        xext = 250
        yext = 350
        num = xext*yext
                
        data = vtkDoubleArray()
        data.SetNumberOfTuples(num)
        data.SetName("data")
        
        ids = vtkIdList()

        self.points = vtkPoints()
        
        count = 0
        for i in range(xext):
            for j in range(yext):
                self.points.InsertNextPoint(i, j, 0)
                data.SetValue(count, i*j)
                ids.InsertNextId(count)
                count += 1

        self.ds = vtkPolyData()
        self.ds.Allocate(xext,yext)
        self.ds.GetPointData().SetScalars(data)
        self.ds.SetPoints(self.points)    
        self.ds.InsertNextCell(vtk.VTK_POLY_VERTEX, ids)
        
        
    def test1(self):
        
        # We have 1000 time steps!
        time = 1000
        
        # Generate the time steps for 1000 days
        timesteps = vtkDoubleArray()
        timesteps.SetNumberOfTuples(time)
        timesteps.SetNumberOfComponents(1)
        for i in range(time):
            timesteps.SetValue(i, 3600*24*i)

        # Create the spatio-temporal source
        timesource = vtkTemporalDataSetSource()
        timesource.SetTimeRange(0, 3600*24*time, timesteps)
        for i in range(time):
            timesource.SetInput(i, self.ds)
        timesource.Update()

        print timesource
        
if __name__ == '__main__':
    suite1 = unittest.TestLoader().loadTestsFromTestCase(vtkTemporalDataSetSourceTest)
    unittest.TextTestRunner(verbosity=2).run(suite1) 
