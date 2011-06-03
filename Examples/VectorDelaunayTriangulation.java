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

public class VectorDelaunayTriangulation {

    static {
        System.loadLibrary("vtkCommonJava");
        System.loadLibrary("vtkFilteringJava");
        System.loadLibrary("vtkIOJava");
        System.loadLibrary("vtkGRASSBridgeIOJava");
        System.loadLibrary("vtkGRASSBridgeVectorJava");
        System.loadLibrary("vtkGRASSBridgeCommonJava");
    }

    public static void main(String[] args) {

        // Initiate grass
        vtkGRASSInit init = new vtkGRASSInit();
        init.Init("VectorDelaunayTriangulation");
        init.ExitOnErrorOn();

        // Now build the pipeline
        // read the vector map without creating topology
        vtkGRASSVectorPolyDataReader reader = new vtkGRASSVectorPolyDataReader();
        reader.SetVectorName("elev_lid792_randpts");

        // start the delaunay triangulation
        vtkDelaunay2D delaunay = new vtkDelaunay2D();
        delaunay.SetInputConnection(reader.GetOutputPort());

        // write the data as XML with base64 encoding
        vtkXMLPolyDataWriter writer = new vtkXMLPolyDataWriter();
        writer.SetFileName("/tmp/test.vtp");
        writer.SetInputConnection(delaunay.GetOutputPort());
        writer.Write();
    }
}
