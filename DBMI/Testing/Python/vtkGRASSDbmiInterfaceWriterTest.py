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

class vtkGRASSDbmiInterfaceWriterTest(unittest.TestCase):
    def setUp(self):
        global firstCheck
        if firstCheck == False:
            #Initiate grass
            init = vtkGRASSInit()
            init.Init("vtkGRASSDbmiInterfaceWriterTest")
            init.ExitOnErrorOn()
            #Remove the vector maps
            inputlist = ["g.remove", "vect=random_points_new"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            # Create the input data
            inputlist = ["v.random", "--o", "-z", "n=100", "zmin=-20", "zmax=2500", "output=random_points_new"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

    def test1(self):
        map = vtkGRASSVectorMapUpdater()
        map.OpenMap("random_points_new", 1)

        rows = map.GetNumberOfPoints()

        db = map.GetDbmiInterface()
	table = vtkGRASSDbmiTable()
        column1 = vtkGRASSDbmiColumn()
        column2 = vtkGRASSDbmiColumn()

        column1.SetName("cat")
        column1.SetSQLTypeToInteger()

        column2.SetName("elev")
        column2.SetSQLTypeToDoublePrecision()

        table.SetName("random_points_new")
        table.AppendColumn(column1)
        table.AppendColumn(column2)

        db.ConnectDBCreateTable(table)

        db.GetTable(table)

        tableEntries = []

        db.BeginTransaction()

        for i in range(rows):
            tableEntries.append(i * i)
            string = "INSERT INTO " + str(table.GetName()) + " values (" +\
                      str(i + 1) + "," + str(tableEntries[i]) + ")"
            print string
            db.ExecuteImmediate(string)

        db.CommitTransaction()

        value = vtkGRASSDbmiValue()
        
        for i in range(rows):
            db.SelectValue(i + 1, column2.GetName(), value)
            val = 0
            if column2.IsValueDouble():
                 val = value.GetDouble()
            elif column2.IsValueInteger():
                val = value.GetInteger()
            elif column2.IsValueString():
                val =  value.GetString()
            else:
                print i + 1, "Unknown datatype at row ", i

            #print val, tableEntries[i]
            self.assertEqual(val, tableEntries[i], "Error writing values to column")

        db.DisconnectDB()
        map.CloseMap(1)


if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(vtkGRASSDbmiInterfaceWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
