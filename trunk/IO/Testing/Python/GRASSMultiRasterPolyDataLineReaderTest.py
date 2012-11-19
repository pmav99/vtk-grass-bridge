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

class GRASSMultiRasterPolyDataLineReaderTest(unittest.TestCase):

    def setUp(self):
        global firstCheck
        if firstCheck == False:
            inputlist = ["g.region", "--o", "n=80", "s=0", "e=180", "w=0", "res=10"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            # Create the input data
            inputlist = ["r.mapcalc", "--o", 'expr=random_tmp=rand(0, 100)']
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

        num = 5
            
        self.maps = vtkStringArray()
        self.maps.SetNumberOfTuples(num)
        self.lineLengths = vtkDoubleArray()
        self.lineLengths.SetNumberOfTuples(num)
            
        
    def test1Points(self):
        init = vtkGRASSInit()
        init.Init("GRASSMultiRasterPolyDataLineReaderTest")
        init.ExitOnErrorOn()
        
        for i in range(5):
            self.maps.SetValue(i, "random_tmp")
            self.lineLengths.SetValue(i, -3*(i + 1))
            
        rs = vtkGRASSMultiRasterPolyDataLineReader()
        rs.SetRasterNames(self.maps)
        rs.SetDataName("RandomValues")
        rs.SetLineLengths(self.lineLengths)

        writer = vtkPolyDataWriter()
        writer.SetFileName("/tmp/GRASSMultiRasterPolyDataLineReaderTest.vtk")
        writer.SetInputConnection(rs.GetOutputPort())
        writer.Update()
        
        print rs.GetOutput()
        
if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSMultiRasterPolyDataLineReaderTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

