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
        init.Init("GRASSVectorMapReaderWriterTest")
        init.ExitOnErrorOn()

    def test1NoTopoWriter(self):
        reader = vtkGRASSVectorMapNoTopoReader()
        reader.OpenMap("nc_state@user1")

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
        print writer
        writer.CloseMap(0)

    def test2TopoWriter(self):
        reader = vtkGRASSVectorMapTopoReader()
        reader.OpenMap("nc_state@user1")

        writer = vtkGRASSVectorMapWriter()
        writer.OpenMap("nc_state_copy_1_1", 0)

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        while reader.ReadNextFeature(points, cats) > 0:
	    writer.WriteFeature(points.GetFeatureType(), points, cats)

        reader.CloseMap()

        writer.SetOrganisation("giscoder.de")
        writer.SetTitle("vtkGRASSBBridgeTest")
        writer.SetPerson("Soeren Gebbert")
        writer.SetCreationDate("28 Sep 2009")
        writer.BuildBase()
        print writer
        writer.BuildAreas()
        print writer
        writer.BuildIsles()
        print writer
        writer.BuildAll()
        print writer
        writer.CloseMap(0)

    def test3TopoUpdate(self):
        reader = vtkGRASSVectorMapTopoReader()
        reader.OpenMap("nc_state@user1")

        writer = vtkGRASSVectorMapWriter()
        writer.SetVectorLevelToTopo()
        writer.OpenMap("nc_state_copy_2", 1)

        writer.SetOrganisation("giscoder.de")
        writer.SetTitle("vtkGRASSBBridgeTest")
        writer.SetPerson("Soeren Gebbert")
        writer.SetCreationDate("28 Sep 2009")

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        while reader.ReadNextFeature(points, cats) > 0:
            writer.WriteFeature(points.GetFeatureType(), points, cats)
        writer.CloseMap(1)
        reader.CloseMap()

        updater = vtkGRASSVectorMapUpdater()
        updater.SetVectorLevelToTopo()
        updater.OpenMap("nc_state_copy_2", 0)
        updater.BuildBase()
        offset = []
        for i  in range(updater.GetNumberOfFeatures()):
            if updater.IsFeatureAlive(i + 1) == 1:
                updater.ReadFeature(i + 1, points, cats)
                offset.append(updater.GetLineOffset(i + 1))
                id = updater.RewriteFeature(i + 1, points.GetFeatureType(), points, cats)
                print "rewrite feature", id, i + 1
        updater.BuildBase()
        for i  in range(updater.GetNumberOfFeatures()):
            if updater.IsFeatureAlive(i + 1) == 1:
                offset.append(updater.GetLineOffset(i + 1))
                updater.DeleteFeature(i + 1)
                print "delete feature", i + 1
        updater.BuildBase()
        for i  in range(updater.GetNumberOfFeatures()):
            if updater.IsFeatureAlive(i + 1) != 1:
                if offset[i] >= 0:
                    updater.RestoreFeature(i + 1, offset[i])
                    print "restore feature", i + 1, offset[i]

        updater.BuildAll()
        updater.RemoveDuplicates(updater.GetFeatureTypeBoundary());
        updater.RemoveDuplicates(updater.GetFeatureTypePoints());
        updater.RemoveDuplicates(updater.GetFeatureTypeLines());
        updater.RemoveSmallAreas(0.001);
        print updater
        updater.CloseMap(1)

    def test4TopoUpdate(self):
        reader = vtkGRASSVectorMapTopoReader()
        reader.OpenMap("nc_state@user1")

        writer = vtkGRASSVectorMapWriter()
        writer.SetVectorLevelToTopo()
        writer.OpenMap("nc_state_copy_3", 1)

        writer.SetOrganisation("giscoder.de")
        writer.SetTitle("vtkGRASSBBridgeTest")
        writer.SetPerson("Soeren Gebbert")
        writer.SetCreationDate("28 Sep 2009")
        
        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        while reader.ReadNextFeature(points, cats) > 0:
	    writer.WriteFeature(points.GetFeatureType(), points, cats)

        writer.BuildBase()

        for i  in range(writer.GetNumberOfFeatures()):
            writer.DeleteFeature(i + 1)
            print "delete feature", i + 1

        reader.CloseMap()
        writer.CloseMap(1)
        reader.OpenMap("nc_state_copy_3")
        print reader



if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorMapReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
