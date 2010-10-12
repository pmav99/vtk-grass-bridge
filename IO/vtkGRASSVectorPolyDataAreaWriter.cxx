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
#include <vtkDataSetAttributes.h>
#include "vtkGRASSVectorPolyDataAreaWriter.h"

#include <vtkCellArray.h>
#include <vtkDataArray.h>
#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "vtkGRASSVectorMapWriter.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include <vtkIdList.h>
#include "vtkGRASSVectorFeatureCats.h"
#include "vtkGRASSVectorMapWriter.h"
#include "vtkGRASSRasterMapWriter.h"
#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkCleanPolyData.h>
#include <vtkExtractEdges.h>
#include <vtkDataArrayCollection.h>
#include "vtkGRASSDbmiInterface.h"
#include "vtkGRASSVectorPolyDataWriter.h"
#include "vtkGRASSDbmiCatValueArray.h"
#include "vtkGRASSDbmiTable.h"
#include "vtkGRASSDbmiColumn.h"
#include "vtkGRASSDbmiValue.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>

vtkCxxRevisionMacro(vtkGRASSVectorPolyDataAreaWriter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkGRASSVectorPolyDataAreaWriter);

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataAreaWriter::vtkGRASSVectorPolyDataAreaWriter()
{
    this->SetNumberOfInputPorts(2);
    this->SetNumberOfOutputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataAreaWriter::~vtkGRASSVectorPolyDataAreaWriter()
{
    ;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorPolyDataAreaWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
        << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
        << (this->Mapset ? this->Mapset : "(none)") << "\n";

}

//----------------------------------------------------------------------------

int
vtkGRASSVectorPolyDataAreaWriter::RequestData(
                                          vtkInformation *vtkNotUsed(request),
                                          vtkInformationVector **inputVector,
                                          vtkInformationVector *vtkNotUsed(outputVector))
{
    // get the info objects
    vtkInformation *in1Info = inputVector[0]->GetInformationObject(0);
    vtkInformation *in2Info = inputVector[1]->GetInformationObject(0);

    // get the input and ouptut
    vtkDataSet *areas = vtkDataSet::SafeDownCast(in1Info->Get(vtkDataObject::DATA_OBJECT()));
    vtkDataSet *centroids = vtkDataSet::SafeDownCast(in2Info->Get(vtkDataObject::DATA_OBJECT()));
    
    int i, j, cat;
    double point[3];
    vtkstd::map<int, int> catmap;
    vtkstd::map<int,int>::iterator catmapit;
    bool hasAreas = false;
    
    if (this->VectorName == NULL)
    {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    VGB_CREATE(vtkGRASSVectorMapWriter, writer);
    
    if (!writer->OpenMap(this->VectorName, 1))
    {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        return -1;
    }

    this->SetMapset(writer->GetMapset());


    // Check for area categories
    vtkDataArray *acategories = NULL;
    if (areas->GetCellData()->HasArray(this->CategoryArrayName))
        acategories = areas->GetCellData()->GetArray(this->CategoryArrayName);
    else {
        vtkErrorMacro( << "Missing category array " << this->CategoryArrayName << " in first input");
        return -1;
    }
    
    // Check for centroid caregories
    vtkDataArray *ccategories = NULL;
    if (centroids->GetCellData()->HasArray(this->CategoryArrayName))
        ccategories = centroids->GetCellData()->GetArray(this->CategoryArrayName);
    else {
        vtkErrorMacro( << "Missing category array " << this->CategoryArrayName << " in second input");
        return -1;
    }
    
    // Create the cat position mapper
    for(i = 0; i < centroids->GetNumberOfCells(); i++) {
        vtkCell *cell = centroids->GetCell(i);
        // Centroids must be of type VTK_VERTEX
        if (cell->GetCellType() == VTK_VERTEX) {
            cat = (int)ccategories->GetTuple1(i);
            //cout << "Map index " << i << " to cat " << cat << endl;
            catmap[cat] = i;
        }
    }

    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, feature);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, centroid);
    VGB_CREATE(vtkIntArray, categories);
    categories->SetNumberOfComponents(1);
    categories->SetName(this->CategoryArrayName);
    
    // We write the data per cell
    for (i = 0; i < areas->GetNumberOfCells(); i++)
    {
        vtkCell *cell = areas->GetCell(i);

        // We check only for vtkPolyData area cells
        if (cell->GetCellType() == VTK_POLYGON || cell->GetCellType() == VTK_TRIANGLE ||cell->GetCellType() == VTK_QUAD)
        {
            hasAreas = true;

            centroid->Reset();
            cats->Reset();

            // Area and centroid share the same category number
            cat = (int)acategories->GetTuple1(i);
            cats->AddCat(1, cat);
            
            // Find the cat
            catmapit = catmap.find(cat);
            if(catmapit == catmap.end())
                continue;
            
            // Store the shared category
            categories->InsertNextValue((double)cat);

            // Get the Index of the centroid
            int idx = catmap[cat];
            
            
            //cout << "index " << idx << " cat " << cat << endl;
            
            vtkCell *ccell = centroids->GetCell(idx);

            // Get the centroid coordiantes, always the first point
            ccell->GetPoints()->GetPoint(0, point);

            centroid->SetFeatureTypeToCentroid();
            centroid->AppendPoint(point[0], point[1], point[2]);
            
            // Attach the centroid
            if (1 > writer->WriteFeature(centroid, cats))
            {
                vtkErrorMacro( << "Error writing centroid to vector map");
                return -1;
            }
        }
        else
            continue;
    }

    // Areas are treated separately
    if(hasAreas) {
        // Clean the data, remove redundant points
        VGB_CREATE(vtkCleanPolyData, cleaner);
        // Extract edges of polygons 
        VGB_CREATE(vtkExtractEdges, edges);

        cleaner->SetInput(areas);
        cleaner->ConvertLinesToPointsOn();
        cleaner->ConvertPolysToLinesOn();
        cleaner->Update();

        edges->SetInput(cleaner->GetOutput());
        edges->Update();

        vtkPolyData *areas = edges->GetOutput();
        
        // We write the data per cell
        for (i = 0; i < areas->GetNumberOfCells(); i++)
        {
            vtkCell *cell = areas->GetCell(i);

            if (cell->GetCellType() == VTK_POLY_LINE || cell->GetCellType() == VTK_LINE)
            {
                feature->Reset();
                // Get the point coordiantes
                vtkPoints *points = cell->GetPoints();
                for (j = 0; j < cell->GetNumberOfPoints(); j++)
                {
                    points->GetPoint(j, point);
                    feature->AppendPoint(point[0], point[1], point[2]);
                } 
                
                feature->SetFeatureTypeToBoundary();
                
                if (1 > writer->WriteFeature(feature, cats))
                {
                    vtkErrorMacro( << "Error writing boundary feature to vector map");
                    return -1;
                }
            }
        }
    }

    // Now add any cell data to the vector database table
    this->AddCellDataToVectorMap(areas->GetCellData(), categories, writer);
    
    if (this->BuildTopo > 0)
        writer->CloseMap(true);
    else
        writer->CloseMap(false);

    return 1;
}
