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

class vtkGRASSDbmiCatValueArrayTest(unittest.TestCase):
    def setUp(self):
        global firstCheck
        if firstCheck == False:
            #Initiate grass
            init = vtkGRASSInit()
            init.Init("vtkGRASSDbmiCatValueArrayTest")
            init.ExitOnErrorOn()
            # Create the input data
            inputlist = ["v.random", "--o", "column=elev", "n=100000", "zmin=20", "zmax=2500", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

    def test1(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("random_points")

        db = map.GetDbmiInterface()

        db.ConnectDB()

        rows = db.GetNumberOfRows()

        print " "
        print "Select all values of elevation lower than 25"

        catval = vtkGRASSDbmiCatValueArray()
        db.SelectCatValueArray("elev", "elev < 25", catval)

        if catval.IsValueTypeDouble():
            for i in range(rows):
                if catval.HasValueDouble(i + 1):
                    print i + 1, catval.GetValueDouble(i + 1)

        if catval.IsValueTypeInteger():
            for i in range(rows):
                if catval.HasValueInteger(i + 1):
                    print i + 1, catval.GetValueInteger(i + 1)

        db.DisconnectDB()

        map.CloseMap()

    def test2(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("random_points")

        db = map.GetDbmiInterface()

        db.ConnectDB()

        rows = db.GetNumberOfRows()

        print " "
        print "Select all values"

        catval = vtkGRASSDbmiCatValueArray()
        db.SelectCatValueArray("elev", catval)
        
#        if catval.IsValueTypeDouble():
#            for i in range(rows):
#                if catval.HasValueDouble(i + 1):
#                    print i + 1, catval.GetValueDouble(i + 1)

        # Convert into double arrays
        cats = vtkIntArray()
        cats.SetNumberOfTuples(rows)
        cats.SetNumberOfComponents(1)
        cats.SetName("cats")

        elev = vtkDoubleArray()
        elev.SetNumberOfTuples(rows)
        elev.SetNumberOfComponents(1)
        elev.SetName("elev")

        # Insert the categories in reverse order
        for i in range(rows):
            cats.InsertValue(i, rows - i)

        catval.ValuesToDoubleArray(cats, elev, -99999)


        # Check
        for i in range(rows):
            if catval.HasValueDouble(cats.GetValue(i)):
                #print cats.GetValue(i), elev.GetValue(i)
                self.assertEqual(catval.GetValueDouble(cats.GetValue(i)), elev.GetValue(i), "Error at cat " + str(cats.GetValue(i)))

        db.DisconnectDB()

        map.CloseMap()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(vtkGRASSDbmiCatValueArrayTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
