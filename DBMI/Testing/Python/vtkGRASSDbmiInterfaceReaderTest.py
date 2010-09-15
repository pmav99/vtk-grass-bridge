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
import subprocess
from libvtkCommonPython import *
from libvtkGRASSBridgeCommonPython import *
from libvtkGRASSBridgeVectorPython import *
from libvtkGRASSBridgeDbmiPython import *

firstCheck = False

class vtkGRASSDbmiInterfaceReaderTest(unittest.TestCase):
    def setUp(self):
        global firstCheck
        if firstCheck == False:
            #Initiate grass
            init = vtkGRASSInit()
            init.Init("vtkGRASSDbmiInterfaceReaderTest")
            init.ExitOnErrorOn()
            # Create the input data
            inputlist = ["v.random", "--o", "column=elev", "n=20", "zmin=-20", "zmax=2500", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

    def test1(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("random_points")

        db = map.GetDbmiInterface()
        value = vtkGRASSDbmiValue()
        
        db.ConnectDB()
	table = vtkGRASSDbmiTable()
	db.GetTable(table)
	print table

        #rows = db.GetNumberOfRows()
        #print "Database table number of rows: ", rows

        for i in range(20):
            db.SelectValue(i + 1, "elev", value)
            print value
            
        print db

        db.DisconnectDB()

        map.CloseMap()


if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(vtkGRASSDbmiInterfaceReaderTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
