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
            inputlist = ["r.mapcalc", "--o", 'expr=random_tmp_1=rand(0, 20)']
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            # Create the input data
            inputlist = ["r.mapcalc", "--o", 'expr=random_tmp_2=rand(20, 40)']
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            # Create the input data
            inputlist = ["r.mapcalc", "--o", 'expr=random_tmp_3=rand(40, 60)']
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

        num = 3
            
        self.maps = vtkStringArray()
        self.maps.SetNumberOfTuples(num)
        self.lineLengths = vtkDoubleArray()
        self.lineLengths.SetNumberOfTuples(num)
            
        
    def test1Points(self):
        init = vtkGRASSInit()
        init.Init("GRASSMultiRasterPolyDataLineReaderTest")
        init.ExitOnErrorOn()
        
        for i in range(3):
            self.maps.SetValue(i, "random_tmp_" + str(i + 1))
            self.lineLengths.SetValue(i, -3*(i + 1))
            
        reader = vtkGRASSMultiRasterPolyDataLineReader()
        reader.SetRasterNames(self.maps)
        reader.SetDataName("RandomValues")
        reader.SetLineLengths(self.lineLengths)
        
        writer = vtkGRASSMultiRasterPolyDataLineWriter()
        writer.SetRasterMapName("multi_raster_test_1")
        writer.SetArrayName("RandomValues")
        writer.SetLayer(1)
        writer.SetInputConnection(reader.GetOutputPort())
        writer.Update()
        
        writer.SetRasterMapName("multi_raster_test_1")
        writer.SetArrayName("RandomValues")
        writer.SetLayer(2)
        writer.SetInputConnection(reader.GetOutputPort())
        writer.Update()
        
        writer.SetRasterMapName("multi_raster_test_1")
        writer.SetArrayName("RandomValues")
        writer.SetLayer(3)
        writer.SetInputConnection(reader.GetOutputPort())
        writer.Update()

        writer2 = vtkPolyDataWriter()
        writer2.SetFileName("/tmp/GRASSMultiRasterPolyDataLineReaderTest.vtk")
        writer2.SetInputConnection(reader.GetOutputPort())
        writer2.Update()
                
if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSMultiRasterPolyDataLineReaderTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

