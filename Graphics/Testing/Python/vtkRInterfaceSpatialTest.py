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
import subprocess
from vtk import *
from libvtkGRASSBridgeFilteringPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
from libvtkGRASSBridgeGraphicsPython import *

firstCheck = False

class vtkRInterfaceSpatialTest(unittest.TestCase):
    
    def setUp(self):    
        # Start the interface
        self.riface = vtkRInterfaceSpatial()
            
        global firstCheck
        if firstCheck == False:
            
            # Start hte interface
            self.riface = vtkRInterfaceSpatial()
        
            #Initiate grass
            init = vtkGRASSInit()
            init.Init("GRASSVectorMapBaseTest")
            init.ExitOnErrorOn()
            # Create the input data
            inputlist = ["v.random", "--o", "n=20", "column=height", "zmin=-20", "zmax=2500", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["r.random.cells", "--o", "output=random_cells", "distance=10.0"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "-l", "input=random_points", "output=random_lines"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "input=random_points", "output=random_areas"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True  
            
        self.buff = ""
        
        for i in range(100):
            self.buff += "                                                                      "
        
        self.riface.OutputBuffer(self.buff, len(self.buff))
        
 
        
    def test1SpatialPointsDataFrame(self):
        
        nrate = vtkDoubleArray()
        nrate.SetNumberOfTuples(25)
        nrate.SetName("nrate")

        sand = vtkDoubleArray()
        sand.SetNumberOfTuples(25)
        sand.SetName("sand")
        
        pd = vtkPolyData()
        pd.Allocate(5,5)
        
        points = vtkPoints()
        ids = vtkIdList()
        
        count = 0
        for i in range(5):
            for j in range(5):
                points.InsertNextPoint(i, j, 0)
                ids.InsertNextId(count)
                nrate.SetValue(count, j)
                sand.SetValue(count, j*j)
                count = count + 1
                
        pd.SetPoints(points)
        pd.InsertNextCell(vtk.VTK_POLY_VERTEX, ids)
        pd.GetPointData().AddArray(nrate)
        pd.GetPointData().AddArray(sand)
        pd.GetPointData().SetActiveScalars(nrate.GetName())     
        
        self.riface.AssignVTKPointSetToRSpatialPointsDataFrame(pd, "spdf")
        
        # Save the workspace for testing
        script = "save(list = ls(all=TRUE), file = \"/home/soeren/vtkRImage1\")"
        print script
        self.riface.EvalRscript(script, True)
        
        return

    def test2SpatialPointsDataFrame(self):
        
        init = vtkGRASSInit()
        init.Init("test2SpatialPointsDataFrame")
        init.ExitOnErrorOn()
        
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("random_points")
        rs.Update()
        
        ctop = vtkCellDataToPointData()
        ctop.SetInputConnection(rs.GetOutputPort())
        ctop.Update()
                
        grassdb = vtkGRASSDatabaseInfo()
        proj4string = grassdb.GetProj4String()
        
        self.riface.SetProj4String(proj4string)
        self.riface.AssignVTKPointSetToRSpatialPointsDataFrame(ctop.GetOutput(), "spdf")
        
        # Save the workspace for testing
        script = "save(list = ls(all=TRUE), file = \"/home/soeren/vtkRImage2\")"
        print script
        self.riface.EvalRscript(script, True)
        
        return

    def test3SpatialGridDataFrame(self):
        
        init = vtkGRASSInit()
        init.Init("test3SpatialGridDataFrame")
        init.ExitOnErrorOn()
        
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.Update()
        
        print rs.GetOutput()
                                
        grassdb = vtkGRASSDatabaseInfo()
        proj4string = grassdb.GetProj4String()
        
        self.riface.SetProj4String(proj4string)
        self.riface.AssignVTKImageDataToRSpatialGridDataFrame(rs.GetOutput(), "sgdf")
        
        # Save the workspace for testing
        script = "save(list = ls(all=TRUE), file = \"/home/soeren/vtkRImage3\")"
        print script
        self.riface.EvalRscript(script, True)
        
        return
        
if __name__ == '__main__':
    suite1 = unittest.TestLoader().loadTestsFromTestCase(vtkRInterfaceSpatialTest)
    unittest.TextTestRunner(verbosity=2).run(suite1) 
