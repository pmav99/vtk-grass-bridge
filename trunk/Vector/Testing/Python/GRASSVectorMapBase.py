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
from vtk import *
from libvtkGRASSBridgeCommonPython import *
from libvtkGRASSBridgeVectorPython import *

firstCheck = False

class GRASSVectorMapBaseTest(unittest.TestCase):
    def setUp(self):
        global firstCheck
        if firstCheck == False:
            #Initiate grass
            init = vtkGRASSInit()
            init.Init("GRASSVectorMapBaseTest")
            init.ExitOnErrorOn()
            # Create the input data
            inputlist = ["v.random", "--o", "-z", "n=20", "zmin=-20", "zmax=2500", "output=random_points"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "-l", "input=random_points", "output=random_lines"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            inputlist = ["v.voronoi", "--o", "input=random_points", "output=random_areas"]
            proc = subprocess.Popen(args=inputlist)
            proc.communicate()
            firstCheck = True

    def test1TopoReader(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("random_areas")

        box = vtkGRASSVectorBBox()
        map.GetBoundingBox(box)
        print map
        print box
        
        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()
        for i in range(map.GetNumberOfFeatures()):
            if map.ReadFeature(i + 1, points, cats) < 0:
                break
            #print points
            #print cats

        map.CloseMap()

    def test2TopoReader(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("random_points")

        box = vtkGRASSVectorBBox()
        map.GetBoundingBox(box)
        print map
        print box

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()
        for i in range(map.GetNumberOfFeatures()):
            if map.ReadFeature(i + 1, points, cats) < 0:
                break
            #print points
            #print cats

        map.CloseMap()

    def test3TopoReader(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("random_lines")

        box = vtkGRASSVectorBBox()
        map.GetBoundingBox(box)
        print map
        print box

        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()
        for i in range(map.GetNumberOfFeatures()):
            if map.ReadFeature(i + 1, points, cats) < 0:
                break
            #print points
            #print cats

        map.CloseMap()

    def test1NoTopoReader(self):
        map = vtkGRASSVectorMapNoTopoReader()
        map.OpenMap("random_areas")

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
        map.OpenMap("random_points")

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
        map.OpenMap("random_lines")

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
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorMapBaseTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
