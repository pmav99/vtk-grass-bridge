/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2011 by Soeren Gebbert, soerengebbert@googlemail.com
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

public class v_sample_rast {

    static {
        System.loadLibrary("vtkCommonJava");
        System.loadLibrary("vtkGRASSBridgeIOJava");
        System.loadLibrary("vtkGRASSBridgeRasterJava");
        System.loadLibrary("vtkGRASSBridgeVectorJava");
        System.loadLibrary("vtkGRASSBridgeCommonJava");
    }

    public static void main(String[] args) {
        // Initiate GRASS
        vtkGRASSInit init = new vtkGRASSInit();
        init.Init("v.sample.rast");
        init.ExitOnErrorOn();

        // Set up the module description
        vtkGRASSModule module = new vtkGRASSModule();
        module.SetDescription("Sample raster maps based on the points of an input vector map");
        module.AddKeyword("vector");
        module.AddKeyword("sample");

        vtkGRASSOptionFactory factory = new vtkGRASSOptionFactory();

        vtkGRASSOption input = factory.CreateInstance(factory.GetVectorInputType());
        vtkGRASSOption output = factory.CreateInstance(factory.GetVectorOutputType());
        vtkGRASSOption raster = factory.CreateInstance(factory.GetRasterInputsType());
        raster.SetKey("raster");

        // Put the command line arguments into a vtk string array and pass it to the parser
        vtkStringArray parameter = new vtkStringArray();

        // We  need to specify the name of the module, otherwise the parser mess up
        parameter.InsertNextValue("v.sample.rast");

        for (int i = 0; i < args.length; i++) {
            parameter.InsertNextValue(args[i]);
        }

        // Use the GRASS GIS messaging interface for noisy output
        vtkGRASSMessagingInterface gm = new vtkGRASSMessagingInterface();

        if (init.Parser(parameter) != true) {
            gm.FatalError("Unable to parse command line");
        }

        // Get the raster input map names
        vtkStringArray rasterNames = new vtkStringArray();
        raster.GetAnswers(rasterNames);

        // Open the input vector map
        vtkGRASSVectorMapTopoReader vectormap = new vtkGRASSVectorMapTopoReader();
        vectormap.OpenMap(input.GetAnswer());

        // Open the output vector map
        vtkGRASSVectorMapWriter outputmap = new vtkGRASSVectorMapWriter();
        outputmap.OpenMap(output.GetAnswer(), 0);

        // Setup the new SQL table for the output map
        vtkGRASSDbmiInterface db = outputmap.GetDbmiInterface();
        vtkGRASSDbmiTable table = new vtkGRASSDbmiTable();
        vtkGRASSDbmiColumn column_cat = new vtkGRASSDbmiColumn();

        column_cat.SetName("cat");
        column_cat.SetSQLTypeToInteger();

        // Attach the category column
        table.SetName(output.GetAnswer());
        table.AppendColumn(column_cat);

        // Attach a new column for each raster map use the raster map name as column name
        for (int map = 0; map < rasterNames.GetNumberOfValues(); map++) {
            String name = rasterNames.GetValue(map);
            vtkGRASSDbmiColumn column_rast = new vtkGRASSDbmiColumn();
            // Assure that the raster map names are SQL compatible column names
            column_rast.SetName(name.replace('.', '_'));
            column_rast.SetSQLTypeToDoublePrecision();
            column_rast.SetNullAllowed();
            column_rast.SetDescription("Sampled raster values");

            table.AppendColumn(column_rast);
        }

        // Create the db table
        db.ConnectDBCreateTable(table);

        db.GetTable(table);

        // Create the sql string buffer
        StringBuilder string = new StringBuilder("");

        // Create the categories
        db.BeginTransaction();
        for (int i = 0; i < vectormap.GetNumberOfFeatures(); i++) {
            string.delete(0, string.length());
            string.append("INSERT INTO ");
            string.append(table.GetName());
            string.append("(cat) values (");
            string.append(i + 1);
            string.append(")");

            //System.out.println(string.toString());

            db.ExecuteImmediate(string.toString());
        }
        // End transaction
        db.CommitTransaction();

        vtkGRASSVectorFeaturePoints points = new vtkGRASSVectorFeaturePoints();
        vtkGRASSVectorFeatureCats cats = new vtkGRASSVectorFeatureCats();
        // Create a new vector category for each found input point
        vtkGRASSVectorFeatureCats newcats = new vtkGRASSVectorFeatureCats();
        vtkDCELL val = new vtkDCELL();
        
        // Sample the data for each raster map
        for (int map = 0; map < rasterNames.GetNumberOfValues(); map++) {

            String name = rasterNames.GetValue(map);
            StringBuilder message = new StringBuilder();
            message.append("Sampling raster map ");
            message.append(name);

            gm.Message(message.toString());

            // The column name
            String sql_name = name.replace('.', '_');

            // Open the raster map
            vtkGRASSRasterMapReader rastermap = new vtkGRASSRasterMapReader();
            rastermap.OpenMap(name);

            // Start the transaction
            db.BeginTransaction();
            for (int i = 0; i < vectormap.GetNumberOfFeatures(); i++) {

                gm.Percent(i, vectormap.GetNumberOfFeatures(), 1);
                
                // Break at the end of the vector file
                if (vectormap.ReadFeature(i + 1, points, cats) < 0) {
                    break;
                }
                // Read only points and add the value to the new vector map
                if (points.IsFeatureTypePoint()) {
                    int cat = i + 1;

                    // Write the new point only once
                    if(map == 0) {
                        newcats.Reset();
                        newcats.AddCat(1, cat);

                        // Append the point and the new category to the output map
                        outputmap.WriteFeature(points, newcats);
                    }

                    double north = points.GetPoint(0)[1];
                    double east = points.GetPoint(0)[0];
                    string.delete(0, string.length());

                    // Sample the raster value and attach found values or null values
                    if (rastermap.GetNearestSampleValue(north, east, val)) {

                        // SQL statement to insert the found value into the output vector table
                        string.append("UPDATE ");
                        string.append(table.GetName());
                        string.append(" SET ");
                        string.append(sql_name);
                        string.append(" = ");
                        string.append(val.GetValueAsDouble());
                        string.append(" where cat = ");
                        string.append(cat);

                        //System.out.println(string.toString());
                    } else {

                        // SQL statement to insert the null value into the output vector table
                        string.append("UPDATE ");
                        string.append(table.GetName());
                        string.append(" SET ");
                        string.append(sql_name);
                        string.append(" = null where cat = ");
                        string.append(cat);

                        //System.out.println(string.toString());
                    }

                    db.ExecuteImmediate(string.toString());
                }
            }
            // End transaction
            db.CommitTransaction();
            // Close raster map
            rastermap.CloseMap();
        }

        gm.Percent(1, 1, 1);

        db.DisconnectDB();

        vectormap.CloseMap();
        outputmap.CloseMap();
    }
}
