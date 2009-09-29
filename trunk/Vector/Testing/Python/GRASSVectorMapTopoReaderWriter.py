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
from libvtkCommonPython import *
from libvtkGRASSBridgeCommonPython import *
from libvtkGRASSBridgeVectorPython import *

class GRASSVectorMapReaderWriterTest(unittest.TestCase):
    def setUp(self):

        #Initiate grass
        init = vtkGRASSInit()

    def test1NoTopoReader(self):
        reader = vtkGRASSVectorMapNoTopoReader()
        reader.OpenMap("nc_state")

        writer = vtkGRASSVectorMapWriter()
        writer.OpenMap("nc_state_copy_1", 0)

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        while reader.ReadNextFeature(points, cats) > 0:
	    writer.WriteFeature(points.GetFeatureType(), points, cats)

        reader.CloseMap()

	writer.SetOrganisation("giscoder.de")
	writer.SetTitle("vtkGRASSBBridgeTest")
	writer.SetPerson("Soeren Gebbert")
	writer.SetCreationDate("28 Sep 2009")
        writer.CloseMap(0)

    def test2NoTopoReader(self):
        reader = vtkGRASSVectorMapNoTopoReader()
        reader.OpenMap("nc_state")

        writer = vtkGRASSVectorMapWriter()
        writer.OpenMap("nc_state_copy_2", 1)

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        while reader.ReadNextFeature(points, cats) > 0:
	    writer.WriteFeature(points.GetFeatureType(), points, cats)

        reader.CloseMap()

	writer.SetOrganisation("giscoder.de")
	writer.SetTitle("vtkGRASSBBridgeTest")
	writer.SetPerson("Soeren Gebbert")
	writer.SetCreationDate("28 Sep 2009")
        writer.CloseMap(1)


if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorMapReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
