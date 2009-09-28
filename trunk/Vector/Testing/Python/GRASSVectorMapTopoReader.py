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

class GRASSVectorMapTopoReaderTest(unittest.TestCase):
    def setUp(self):

        #Initiate grass
        init = vtkGRASSInit()

    def test1TopoReader(self):
        map = vtkGRASSVectorMapTopoReader()
        map.OpenMap("nc_state@user1")

        box = vtkGRASSVectorBBox()
        map.GetBoundingBox(box)
        print box
        print map

        print "Lines: ", map.GetNumberOfLines()
        print "Boundaries: ", map.GetNumberOfBoundaries()
        print "Points: ", map.GetNumberOfPoints()
        print "Nodes: ", map.GetNumberOfNodes()
        print "Areas: ", map.GetNumberOfAreas()
        print "Isles: ", map.GetNumberOfIsles()
        print "Faces: ", map.GetNumberOfFaces()
        print "Kernels: ", map.GetNumberOfKernels()
        print "Centroids: ", map.GetNumberOfCentroids()
        
        points = vtkGRASSVectorFeaturePoints()
        cats = vtkGRASSVectorFeatureCats()

        for i in range(map.GetNumberOfFeatures()):
            if map.ReadFeature(i + 1, points, cats) < 0:
                break
            print points.GetFeatureTypeAsString()
            #print cats

        print "Areas"

        for i in range(map.GetNumberOfAreas()):
            if map.GetArea(i + 1, points, cats) < 0:
                break
            print points.GetFeatureTypeAsString()
            #print points
            #print cats
        

        map.CloseMap()


if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSVectorMapTopoReaderTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
