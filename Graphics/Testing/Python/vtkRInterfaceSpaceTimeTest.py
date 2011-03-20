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

class vtkRInterfaceSpaceTimeTest(unittest.TestCase):
    
    def setUp(self):    
        # Start the interface
        self.riface = vtkRInterfaceSpaceTime()
            
        global firstCheck
        if firstCheck == False:
            
            # Start the interface
            self.riface = vtkRInterfaceSpaceTime()
        
            #Initiate grass
            init = vtkGRASSInit()
            init.Init("GRASSVectorMapBaseTest")
            init.ExitOnErrorOn()
            
            # Create the input raster and vector data 
            inputlist = ["v.random", "--o", "n=20", "column=height", "zmin=-20", "zmax=2500", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            
            inputlist = ["r.random.cells", "--o", "output=random_cells", "distance=10.0"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            
            firstCheck = True
            

    def test1SpaceTimePointsDataFrame(self):
        
        init = vtkGRASSInit()
        init.Init("test1SpaceTimePointsDataFrame")
        init.ExitOnErrorOn()
        
        # Read the vector points from GRASS GIS
        rs = vtkGRASSVectorPolyDataReader()
        rs.SetVectorName("random_points")
        rs.Update()
        
        # The generated poly-data has only cell data attached
        # so we need the VTK cell data to point data converter
        ctop = vtkCellDataToPointData()
        ctop.SetInputConnection(rs.GetOutputPort())
        ctop.Update()
        
        # We generate three time steps 0d, 1d, 2d 
        timeSteps = vtkDoubleArray()
        timeSteps.InsertNextValue(0)
        timeSteps.InsertNextValue(3600*24)
        timeSteps.InsertNextValue(3600*48)
        
        # The temporal data set source is used to create valid 
        # and consistent temporal data sets
        # Here we have three data sets attached for
        # each time stept in the timeSteps array
        tds = vtkTemporalDataSetSource()
        tds.SetTimeRange(0, 3600*48, timeSteps)
        tds.SetInput(0, ctop.GetOutput())
        tds.SetInput(1, ctop.GetOutput())
        tds.SetInput(2, ctop.GetOutput())
        tds.Update()
        
        # We need the projection string for R spatial objects
        grassdb = vtkGRASSDatabaseInfo()
        proj4string = grassdb.GetProj4String()
        
        # Set the projection and convert the temporal data set into
        # a spatio-temporal full data frame. Start date is 17. March 2011
        self.riface.SetProj4String(proj4string)
        self.riface.AssignVTKTemporalDataSetToRSTFDF(tds.GetOutput(), "stfdf1", timeSteps, "2011-03-17")
        
        # Save the workspace for testing
        script = "save(list = ls(all=TRUE), file = \"/home/soeren/vtkRSpacetimeImage1\")"
        print script
        self.riface.EvalRscript(script, True)
        
        return

    def test2SpaceTimeGridDataFrame(self):
        
        init = vtkGRASSInit()
        init.Init("test2SpaceTimeGridDataFrame")
        init.ExitOnErrorOn()
        
        # Read the generated raster map from GRASS GIS
        rs = vtkGRASSRasterImageReader()
        rs.SetRasterName("random_cells")
        rs.Update()
        
        # We generate five time steps 0d, 1d, 2d, 3d, 4d
        timeSteps = vtkDoubleArray()
        timeSteps.InsertNextValue(0)
        timeSteps.InsertNextValue(3600*24)
        timeSteps.InsertNextValue(3600*48)
        timeSteps.InsertNextValue(3600*72)
        timeSteps.InsertNextValue(3600*96)
        
        # The temporal data set source is used to create valid 
        # and consistent temporal data sets
        # Here we have five data sets attached for
        # each time stept in the timeSteps array        
        tds = vtkTemporalDataSetSource()
        tds.SetTimeRange(0, 3600*96, timeSteps)
        tds.SetInput(0, rs.GetOutput())
        tds.SetInput(1, rs.GetOutput())
        tds.SetInput(2, rs.GetOutput())
        tds.SetInput(3, rs.GetOutput())
        tds.SetInput(4, rs.GetOutput())
        tds.Update()
                
        # We need the projection string for R spatial objects
        grassdb = vtkGRASSDatabaseInfo()
        proj4string = grassdb.GetProj4String()
        
        # Set the projection and convert the temporal data set into
        # a spatio-temporal full data frame. Start date is 17. March 2011
        self.riface.SetProj4String(proj4string)
        self.riface.AssignVTKTemporalDataSetToRSTFDF(tds.GetOutput(), "stfdf2", timeSteps, "2011-03-17")
        
        # Save the workspace for testing
        script = "save(list = ls(all=TRUE), file = \"/home/soeren/vtkRSpacetimeImage2\")"
        print script
        self.riface.EvalRscript(script, True)
        
        return
        
if __name__ == '__main__':
    suite1 = unittest.TestLoader().loadTestsFromTestCase(vtkRInterfaceSpaceTimeTest)
    unittest.TextTestRunner(verbosity=2).run(suite1) 
