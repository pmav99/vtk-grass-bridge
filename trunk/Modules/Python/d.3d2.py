#!/usr/bin/env python
#
############################################################################
#
# MODULE:       d.3d2
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Visualize an elevation map and a vector map as overlay using
#               VTK render window
#
# COPYRIGHT:    (C) 2010 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#include the grass, VTK and vtkGRASSBridge Python libraries
from libvtkCommonPython import *
from libvtkHybridPython import *
from libvtkFilteringPython import *
from libvtkRenderingPython import *
from libvtkGraphicsPython import *
from libvtkIOPython import *
from libvtkImagingPython import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *

from vtkGRASSBridgeModuleBase import *
import sys

def main():
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("d.3d2")
    init.ExitOnErrorOn()

    module = vtkGRASSModule()
    module.SetDescription("Visualize an elevation map and a vector map as overlay usingv VTK render window")
    module.AddKeyword("raster")
    module.AddKeyword("vector")
    module.AddKeyword("vtk")

    raster =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputType(), "elev")
    vector =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorInputType(), "overlay")
    feature = vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetVectorFeatureType())
    feature.SetDefaultOptions("line,boundary")
    feature.SetDefaultAnswer("line,boundary")

    scale = vtkGRASSOption()
    scale.SetKey("scale")
    scale.SetDefaultAnswer("1")
    scale.RequiredOff()
    scale.MultipleOff()
    scale.SetTypeToDouble()
    scale.SetDescription("Scale factor of height data")

    # Put the command line arguments into a vtk string array and pass it to the parser
    paramter = vtkStringArray()
    for arg in sys.argv:
        paramter.InsertNextValue(str(arg))

    init.Parser(paramter)

    # Use the GRASS GIS messaging interface for gm and noisy output
    gm = vtkGRASSMessagingInterface()

    # Set up the vector reader
    vreader = vtkGRASSVectorTopoPolyDataReader()
    vreader.SetVectorName(vector.GetAnswer())
    vreader.ReadDBTableOff()

    vdata = vtkAppendPolyData()
    data = vtkPolyData()
    
    for name in feature.GetAnswer().split(","):
        if name == "point":
            gm.Message("Reading feature point")
            vreader.SetFeatureTypeToPoint()
            vreader.Update()
            if vreader.GetOutput().GetNumberOfCells() > 0:
                data.DeepCopy(vreader.GetOutput())
                vdata.AddInput(data)
        if name == "line":
            gm.Message("Reading feature line")
            vreader.SetFeatureTypeToLine()
            vreader.Update()
            if vreader.GetOutput().GetNumberOfCells() > 0:
                data.DeepCopy(vreader.GetOutput())
                vdata.AddInput(data)
        if name == "boundary":
            gm.Message("Reading feature boundary")
            vreader.SetFeatureTypeToBoundary()
            vreader.Update()
            if vreader.GetOutput().GetNumberOfCells() > 0:
                data.DeepCopy(vreader.GetOutput())
                vdata.AddInput(data)
        if name == "centroid":
            gm.Message("Reading feature centroid")
            vreader.SetFeatureTypeToCentroid()
            vreader.Update()
            if vreader.GetOutput().GetNumberOfCells() > 0:
                data.DeepCopy(vreader.GetOutput())
                vdata.AddInput(data)

    del data
    vdata.Update()

    lut = vtkLookupTable()
    lut.SetHueRange( 0.6, 0)
    lut.SetSaturationRange( 1.0, 0)
    lut.SetValueRange( 0.5, 1.0)

    rreader = vtkGRASSRasterImageReader()
    rreader.SetRasterName(raster.GetAnswer())
    rreader.UseCurrentRegion()
   
    # Scale the image with the scale factor
    height = vtkImageShiftScale()
    height.SetInputConnection(rreader.GetOutputPort())
    height.SetShift(0)
    height.SetScale(float(scale.GetAnswer()))

    # Get the range for color table
    range = [0.0,0.0]
    height.Update()
    height.GetOutput().GetPointData().GetScalars().GetRange(range)

    # Clip the vector data based on the image x,y bounding box
    bounds = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    height.GetOutput().GetBounds(bounds)
    xMax = bounds[1]
    xMin = bounds[0]
    yMax = bounds[3]
    yMin = bounds[2]
    zMax = 999999
    zMin = -999999   
    box = vtkBox() 
    box.SetBounds (xMin, xMax, yMin, yMax, zMin, zMax)

    vectors = vtkClipPolyData()
    vectors.SetInputConnection(vdata.GetOutputPort())
    vectors.SetClipFunction(box)
    vectors.InsideOutOn()
    
    # Create a surface based on the height raster map
    surface = vtkImageDataGeometryFilter()
    surface.SetInputConnection(height.GetOutputPort())
    
    # Attach the height values to the grid points
    warp = vtkWarpScalar()
    warp.SetInputConnection(surface.GetOutputPort())
    warp.SetScaleFactor(1)
    warp.UseNormalOn()
    warp.SetNormal(0, 0, 1)

    # Compute normals for shading
    normals = vtkPolyDataNormals()
    normals.SetInput(warp.GetPolyDataOutput())
    normals.SetFeatureAngle(60)
    normals.SplittingOff()
    
    demMapper = vtkPolyDataMapper()
    demMapper.SetInputConnection(normals.GetOutputPort())
    demMapper.SetScalarRange(range[0], range[1])
    demMapper.SetLookupTable( lut)

    demActor = vtkLODActor()
    demActor.SetMapper( demMapper)

    projectedPaths = vtkProjectedTerrainPath()
    projectedPaths.SetInput(vectors.GetOutput())
    projectedPaths.SetSource(height.GetOutput())
    projectedPaths.SetHeightOffset(5)
    projectedPaths.SetHeightTolerance(1)
    projectedPaths.SetProjectionModeToNonOccluded()
    projectedPaths.SetProjectionModeToHug()

    pathMapper = vtkPolyDataMapper()
    pathMapper.SetInputConnection(projectedPaths. GetOutputPort())

    paths = vtkActor()
    paths.SetMapper( pathMapper)
    paths.GetProperty().SetColor( 1, 0, 0)
    paths.GetProperty().SetLineWidth (2)

    ren1 = vtkRenderer()
    renWin = vtkRenderWindow()
    renWin.AddRenderer( ren1)
    renWin.SetSize(800, 600)
    iren = vtkRenderWindowInteractor()
    iren. SetRenderWindow( renWin)

    ren1.AddActor(demActor)
    ren1.AddActor(paths)
    ren1.SetBackground(.9,.9,.9)

    ren1.ResetCamera()
    ren1.ResetCameraClippingRange()

    iren.Initialize()
    renWin.Render()
    iren.Start()

 
if __name__ == "__main__":
    main()
