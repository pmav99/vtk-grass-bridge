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

class GRASSVectorLinePointsTest(unittest.TestCase):
    def setUp(self):

        #Initiate grass
        init = vtkGRASSInit()

    def test1TopoReader(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("nc_state")

        box = vtkGRASSVectorBBox()
        map.GetBoundingBox(box)
        print map
        print box
        
        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()
        for i in range(map.GetNumberOfFeatures()):
            if map.ReadFeature(points, cats, i + 1) < 0:
                break
            #print points
            #print cats

        map.CloseMap()

    def test2TopoReader(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("elev_lidrural_mrpts")

        box = vtkGRASSVectorBBox()
        map.GetBoundingBox(box)
        print map
        print box

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()
        for i in range(map.GetNumberOfFeatures()):
            if map.ReadFeature(points, cats, i + 1) < 0:
                break
            #print points
            #print cats

        map.CloseMap()

    def test3TopoReader(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("streams")

        box = vtkGRASSVectorBBox()
        map.GetBoundingBox(box)
        print map
        print box

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()
        for i in range(map.GetNumberOfFeatures()):
            if map.ReadFeature(points, cats, i + 1) < 0:
                break
            #print points
            #print cats

        map.CloseMap()

    def test1NoTopoReader(self):
        map = vtkGRASSVectorMapNoTopoReader()
        map.OpenMap("nc_state")

        print map

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        count = 0
        while map.ReadNextFeature(points, cats) > 0:
            #print points
            #print cats
            count += 1
            #print count

        map.CloseMap()

    def test2NoTopoReader(self):
        map = vtkGRASSVectorMapNoTopoReader()
        map.OpenMap("elev_lidrural_mrpts")

        print map

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        count = 0
        while map.ReadNextFeature(points, cats) > 0:
            #print points
            #print cats
            count += 1
            #print count

        map.CloseMap()

    def test3NoTopoReader(self):
        map = vtkGRASSVectorMapNoTopoReader()
        map.OpenMap("streams")

        print map

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        count = 0
        while map.ReadNextFeature(points, cats) > 0:
            #print points
            #print cats
            count += 1
            #print count

        map.CloseMap()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorLinePointsTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
