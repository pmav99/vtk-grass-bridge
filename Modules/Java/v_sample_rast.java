/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

import vtk.*;

/**
 *
 * @author soeren
 */
public class v_sample_rast {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
    // Initiate GRASS
    vtkGRASSInit init = new vtkGRASSInit();
    init.Init("v.sample.rast");
    init.ExitOnErrorOn();

    vtkGRASSModule module = new vtkGRASSModule();
    module.SetDescription("Sample a raster map based on the points of an input vector map");
    module.AddKeyword("vector");
    module.AddKeyword("sample");

    vtkGRASSOptionFactory factory = new vtkGRASSOptionFactory();

    vtkGRASSOption input =  factory.CreateInstance(factory.GetVectorInputType());
    vtkGRASSOption output =  factory.CreateInstance(factory.GetVectorOutputType());
    vtkGRASSOption raster =  factory.CreateInstance(factory.GetRasterInputType());
    raster.SetKey("raster");

    vtkGRASSOption column = factory.CreateInstance(factory.GetDataBaseColumnType());
    column.SetDescription("Name of the column added to the output vector map");
    column.SetDefaultAnswer("rast_val");

    // Put the command line arguments into a vtk string array and pass it to the parser
    vtkStringArray parameter = new vtkStringArray();

    // Use the GRASS GIS messaging interface for gm and noisy output
    vtkGRASSMessagingInterface gm = new vtkGRASSMessagingInterface();

    for(int i = 0; i < args.length; i++)
        parameter.InsertNextValue(args[i]);

    if(init.Parser(parameter) != true)
        gm.FatalError("Unable to parse command line");

    // Open the raster map
    vtkGRASSRasterMapReader rastermap = new vtkGRASSRasterMapReader();
    rastermap.OpenMap(raster.GetAnswer());

    // Open the input vector map
    vtkGRASSVectorMapTopoReader vectormap = new vtkGRASSVectorMapTopoReader();
    vectormap.OpenMap(input.GetAnswer());

    // Open the output vector map
    vtkGRASSVectorMapWriter outputmap = new vtkGRASSVectorMapWriter();
    outputmap.OpenMap(output.GetAnswer());

    // Setup the new SQL table for the output map
    vtkGRASSDbmiInterface db = outputmap.GetDbmiInterface();
    vtkGRASSDbmiTable table = new vtkGRASSDbmiTable();
    vtkGRASSDbmiColumn column1 = new vtkGRASSDbmiColumn();
    vtkGRASSDbmiColumn column2 = new vtkGRASSDbmiColumn();

    column1.SetName("cat");
    column1.SetSQLTypeToInteger();

    column2.SetName(column.GetAnswer());
    column2.SetSQLTypeToDoublePrecision();

    table.SetName(output.GetAnswer());
    table.AppendColumn(column1);
    table.AppendColumn(column2);

    // Create the db table
    db.ConnectDBCreateTable(table);

    db.GetTable(table);

    vtkGRASSVectorFeaturePoints points = new vtkGRASSVectorFeaturePoints();
    vtkGRASSVectorFeatureCats cats = new vtkGRASSVectorFeatureCats();

    // Start the transaction
    db.BeginTransaction();

    int cat = 1;
    for(int i = 0; i < vectormap.GetNumberOfFeatures(); i++) {
        gm.Percent(i, vectormap.GetNumberOfFeatures(), 1);
        // Break at the end of the vector file
        if(vectormap.ReadFeature(i + 1, points, cats) < 0)
            break;
        // Read only points and add the value to the new vector map
        if(points.IsFeatureTypePoint()) {
            
            vtkDouble val = new vtkDouble();
            double p[] = points.GetPoint(0);
            // Sample the raster value
            if(rastermap.GetNearestSampleValue(p[0], p[1], val)) {

                // Create a new vector category for each found sample
                vtkGRASSVectorFeatureCats newcats = new vtkGRASSVectorFeatureCats();
                newcats.AddCat(1, cat);

                // Append the point and the new category to the output map
                outputmap.WriteFeature(points, newcats);

                // SQL statement to insert the found value into the output vector table
                StringBuffer string = new StringBuffer();
                string.append("INSERT INTO ");
                string.append(table.GetName());
                string.append(" values (");
                string.append(cat);
                string.append(",");
                string.append(val.GetValue());
                string.append(")");

                db.ExecuteImmediate(string.toString());

                // Next category
                cat += 1;
            }
        }
    }

    // End transaction
    db.CommitTransaction();

    gm.Percent(1, 1, 1);

    db.DisconnectDB();

    vectormap.CloseMap();
    rastermap.CloseMap();
    outputmap.CloseMap();
    }
}
