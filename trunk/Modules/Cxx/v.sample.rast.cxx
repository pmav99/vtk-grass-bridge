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

#include <vtkStringArray.h>
#include <vtkGRASSDbmiColumn.h>
#include <vtkGRASSInit.h>
#include <vtkGRASSModule.h>
#include <vtkGRASSOptionFactory.h>
#include <vtkGRASSOption.h>
#include <vtkGRASSMessagingInterface.h>
#include <vtkGRASSVectorMapTopoReader.h>
#include <vtkGRASSVectorMapWriter.h>
#include <vtkGRASSDbmiInterface.h>
#include <vtkGRASSDbmiTable.h>
#include <vtkGRASSDbmiColumn.h>
#include <vtkGRASSVectorFeaturePoints.h>
#include <vtkGRASSVectorFeatureCats.h>
#include <vtkGRASSRasterMapReader.h>

#include <string>

int main(int argc, char **argv)
{
    // Initiate GRASS
    VGB_CREATE(vtkGRASSInit, init);
    init->Init("v.sample.rast");
    init->ExitOnErrorOn();
    char buff[1024];

    // Set up the module description
    VGB_CREATE(vtkGRASSModule, module);
    module->SetDescription("Sample raster maps based on the points of an input vector map");
    module->AddKeyword("vector");
    module->AddKeyword("sample");

    vtkGRASSOption *input = vtkGRASSOptionFactory::CreateInstance(vtkGRASSOptionFactory::GetVectorInputType());
    vtkGRASSOption *output = vtkGRASSOptionFactory::CreateInstance(vtkGRASSOptionFactory::GetVectorOutputType());
    vtkGRASSOption *raster = vtkGRASSOptionFactory::CreateInstance(vtkGRASSOptionFactory::GetRasterInputsType());
    raster->SetKey("raster");

    // Use the GRASS GIS messaging interface for noisy output
    VGB_CREATE(vtkGRASSMessagingInterface, gm);

    if (init->Parser(argc, argv) != true) {
        G_snprintf(buff, 1024, "Unable to parse command line");
        gm->FatalError(buff);
    }

    // Get the raster input map names
    VGB_CREATE(vtkStringArray, rasterNames);
    raster->GetAnswers(rasterNames);

    // Open the input vector map
    VGB_CREATE(vtkGRASSVectorMapTopoReader, vectormap);
    vectormap->OpenMap(input->GetAnswer());

    // Open the output vector map
    VGB_CREATE(vtkGRASSVectorMapWriter, outputmap);
    outputmap->OpenMap(output->GetAnswer(), 0);

    // Setup the new SQL table for the output map
    vtkGRASSDbmiInterface *db = outputmap->GetDbmiInterface();
    VGB_CREATE(vtkGRASSDbmiTable, table);
    VGB_CREATE(vtkGRASSDbmiColumn, column_cat);

    column_cat->SetName("cat");
    column_cat->SetSQLTypeToInteger();

    // Attach the category column
    table->SetName(output->GetAnswer());
    table->AppendColumn(column_cat);

    // Attach a new column for each raster map use the raster map name as column name
    for (int map = 0; map < rasterNames->GetNumberOfValues(); map++) {
        std::string name = rasterNames->GetValue(map);
        VGB_CREATE(vtkGRASSDbmiColumn, column_rast);
        // Assure that the raster map names are SQL compatible column names
        std::string sql_name = name;
        for(unsigned int pos = 0; pos < name.size(); pos++) {
            if(sql_name[pos] == '.')
                sql_name[pos] = '_';
        }
        
        column_rast->SetName(sql_name.c_str());
        column_rast->SetSQLTypeToDoublePrecision();
        column_rast->SetNullAllowed();
        column_rast->SetDescription("Sampled raster values");

        table->AppendColumn(column_rast);
    }

    // Create the db table
    db->ConnectDBCreateTable(table);

    db->GetTable(table);

    // Create the string buffer
    std::string sqlstring;

    // Create the categories
    db->BeginTransaction();
    for (int i = 0; i < vectormap->GetNumberOfFeatures(); i++) {
        sqlstring.clear();
        sqlstring.append("INSERT INTO ");
        sqlstring.append(table->GetName());
        sqlstring.append("(cat) values (");
        G_snprintf(buff, 1024, "%d", i + 1);
        sqlstring.append(buff);
        sqlstring.append(")");

        db->ExecuteImmediate(sqlstring.c_str());
    }
    // End transaction
    db->CommitTransaction();

    VGB_CREATE(vtkGRASSVectorFeaturePoints, points);
    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);
    // Create a new vector category for each found input point
    VGB_CREATE(vtkGRASSVectorFeatureCats, newcats);
    VGB_CREATE(vtkDCELL, val);

    // Sample the data for each raster map
    for (int map = 0; map < rasterNames->GetNumberOfValues(); map++) {
        G_snprintf(buff, 1024, "Sampling raster map %s", rasterNames->GetValue(map).c_str());
        gm->Message(buff);

        // The column name
        std::string sql_name = rasterNames->GetValue(map);
        for(unsigned int pos = 0; pos < sql_name.size(); pos++) {
            if(sql_name[pos] == '.')
                sql_name[pos] = '_';
        }

        // Open the raster map
        VGB_CREATE(vtkGRASSRasterMapReader, rastermap);
        rastermap->OpenMap(rasterNames->GetValue(map).c_str());

        // Start the transaction
        db->BeginTransaction();
        for (int i = 0; i < vectormap->GetNumberOfFeatures(); i++) {

            gm->Percent(i, vectormap->GetNumberOfFeatures(), 1);

            // Break at the end of the vector file
            if (vectormap->ReadFeature(i + 1, points, cats) < 0) {
                break;
            }
            // Read only points and add the value to the new vector map
            if (points->IsFeatureTypePoint()) {
                int cat = i + 1;

                // Write the new point only once
                if(map == 0) {
                    newcats->Reset();
                    newcats->AddCat(1, cat);

                    // Append the point and the new category to the output map
                    outputmap->WriteFeature(points, newcats);
                }

                double north = points->GetPoint(0)[1];
                double east = points->GetPoint(0)[0];
                sqlstring.clear();

                // Sample the raster value and attach found values or null values
                if (rastermap->GetNearestSampleValue(north, east, val)) {

                    // SQL statement to insert the found value into the output vector table
                    sqlstring.append("UPDATE ");
                    sqlstring.append(table->GetName());
                    sqlstring.append(" SET ");
                    sqlstring.append(sql_name.c_str());
                    sqlstring.append(" = ");
                    G_snprintf(buff, 32, "%g", val->GetValueAsDouble());
                    sqlstring.append(buff);
                    sqlstring.append(" where cat = ");
                    G_snprintf(buff, 32, "%d", cat);
                    sqlstring.append(buff);
                } else {

                    // SQL statement to insert the null value into the output vector table
                    sqlstring.append("UPDATE ");
                    sqlstring.append(table->GetName());
                    sqlstring.append(" SET ");
                    sqlstring.append(sql_name.c_str());
                    sqlstring.append(" = null where cat = ");
                    G_snprintf(buff, 32, "%d", cat);
                    sqlstring.append(buff);
                }
                db->ExecuteImmediate(sqlstring.c_str());
            }
        }
        // End transaction
        db->CommitTransaction();
        // Close raster map
        rastermap->CloseMap();
    }

    gm->Percent(1, 1, 1);

    db->DisconnectDB();

    vectormap->CloseMap();
    outputmap->CloseMap();
    
    vtkIndent indent;
    
    input->PrintSelf(cout, indent);
    raster->PrintSelf(cout, indent);
    output->PrintSelf(cout, indent);
    
    input->Delete();
    raster->Delete();
    output->Delete();
}
