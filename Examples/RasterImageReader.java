//  Program: vtkGRASSBridge
//  COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; version 2 of the License.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
import vtk.*;

public class RasterImageReader {

  static {
    System.loadLibrary("vtkCommonJava");
    System.loadLibrary("vtkFilteringJava");
    System.loadLibrary("vtkIOJava");
    System.loadLibrary("vtkImagingJava");
    System.loadLibrary("vtkGraphicsJava");
    System.loadLibrary("vtkRenderingJava");
    System.loadLibrary("vtkGRASSBridgeIOJava");
    System.loadLibrary("vtkGRASSBridgeRasterJava");
    System.loadLibrary("vtkGRASSBridgeVectorJava");
    System.loadLibrary("vtkGRASSBridgeCommonJava");
  }

    public static void main(String[] args) {

        // Initiate grass
        vtk.vtkGRASSInit init = new vtk.vtkGRASSInit();
        init.Init("RasterImageReader");
        init.ExitOnErrorOn();
        // List all raster maps
        vtkGRASSRasterMapLister maplister = new vtkGRASSRasterMapLister();
        String name = maplister.GetFileNames().GetValue(0);
        
        // Choose the first raster map in the list (which is hopefully not empty)
        vtkGRASSRasterImageReader rs = new vtkGRASSRasterImageReader();
        rs.SetRasterName(name);
        // Use the region of the raster map
        rs.UseRasterRegion();
        rs.Update();
        vtkImageGaussianSmooth filter = new vtkImageGaussianSmooth();
        filter.SetInputConnection(rs.GetOutputPort());
        
        // We display the raster image in the first window
        // and the processed image in the second window
        
        // Create mouse interaction for the first viewer
        vtkRenderWindowInteractor viewInt1 = new vtkRenderWindowInteractor();
        // Create the first image viewer
        vtkImageViewer2 viewer1 = new vtkImageViewer2();
        viewer1.SetInputConnection(rs.GetOutputPort());
        viewer1.SetColorWindow(255);
        viewer1.SetupInteractor(viewInt1);
        viewer1.Render();
        // Create mouse interaction for the second viewer
        vtkRenderWindowInteractor viewInt2 = new vtkRenderWindowInteractor();
        // Create the second image viewer
        vtkImageViewer2 viewer2 = new vtkImageViewer2();
        viewer2.SetInputConnection(filter.GetOutputPort());
        viewer2.SetColorWindow(255);
        viewer2.SetupInteractor(viewInt2);
        viewer2.Render();
        // Start the interaction
        viewInt1.Start();
        viewInt2.Start();

    }
}
