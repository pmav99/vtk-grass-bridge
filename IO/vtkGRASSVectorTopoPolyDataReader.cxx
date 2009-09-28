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
#include "vtkGRASSVectorTopoPolyDataReader.h"

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "vtkGRASSVectorMapTopoReader.h"
#include "vtkGRASSVectorMapNoTopoReader.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include <vtkIdList.h>
#include "vtkGRASSVectorFeatureCats.h"


vtkCxxRevisionMacro(vtkGRASSVectorTopoPolyDataReader, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkGRASSVectorTopoPolyDataReader);

//----------------------------------------------------------------------------

vtkGRASSVectorTopoPolyDataReader::vtkGRASSVectorTopoPolyDataReader() {
    this->Mapset = NULL;
    this->VectorName = NULL;
    this->FeatureType = GV_POINTS;
    this->SetNumberOfInputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSVectorTopoPolyDataReader::~vtkGRASSVectorTopoPolyDataReader() {
    if (this->VectorName)
        delete [] this->VectorName;
    if (this->Mapset)
        delete [] this->Mapset;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorTopoPolyDataReader::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
            << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
            << (this->Mapset ? this->Mapset : "(none)") << "\n";

}

//----------------------------------------------------------------------------

int
vtkGRASSVectorTopoPolyDataReader::RequestData(vtkInformation*,
        vtkInformationVector**,
        vtkInformationVector* outputVector) {
    // Allocate objects to hold points and vertex cells.
    vtkPoints *points = vtkPoints::New();

    if (this->VectorName == NULL) {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    vtkGRASSVectorMapTopoReader *reader = vtkGRASSVectorMapTopoReader::New();
    vtkGRASSVectorFeatureCats *cats = vtkGRASSVectorFeatureCats::New();
    vtkGRASSVectorFeaturePoints *feature = vtkGRASSVectorFeaturePoints::New();


    if (!reader->OpenMap(this->VectorName)) {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        reader->Delete();
        cats->Delete();
        feature->Delete();
        return -1;
    }

    this->SetMapset(reader->GetMapset());

    vtkPolyData* output = vtkPolyData::GetData(outputVector);
    output->Allocate(1);

    vtkIntArray *categories = vtkIntArray::New();
    categories->SetNumberOfComponents(1);
    categories->SetName("cat");

    vtkIdList *ids = vtkIdList::New();
    int i, area;
    vtkIdType id;

    // Read only the requested feature in vector map
    if (this->FeatureType != GV_AREA) {
        while (reader->ReadNextFeature(feature, cats) > 0) {
            if (feature->GetFeatureType() == this->FeatureType) {
                for (i = 0; i < feature->GetNumberOfPoints(); i++) {
                    double *point = feature->GetPoint(i);
                    id = points->InsertNextPoint(point[0], point[1], point[2]);
                    ids->InsertNextId(id);
                }
                output->InsertNextCell(feature->GetVTKCellId(), ids);
                ids->Initialize();

                categories->InsertNextValue(cats->GetCat(1));
            }
        }
    } else {
        for (area = 1; area <= reader->GetNumberOfAreas(); area++) {
            reader->GetArea(area, feature, cats);
            for (i = 0; i < feature->GetNumberOfPoints(); i++) {
                double *point = feature->GetPoint(i);
                id = points->InsertNextPoint(point[0], point[1], point[2]);
                ids->InsertNextId(id);
            }
            output->InsertNextCell(feature->GetVTKCellId(), ids);
            ids->Initialize();

            categories->InsertNextValue(cats->GetCat(1));
        }
    }

    ids->Delete();
    vtkDebugMacro("Read " << points->GetNumberOfPoints() << " points.");

    // Store the points and cells in the output data object.
    output->SetPoints(points);
    output->GetCellData()->SetScalars(categories);

    vtkIndent indent;
    reader->PrintSelf(cout, indent);

    categories->Delete();
    points->Delete();
    reader->Delete();
    cats->Delete();
    feature->Delete();

    return 1;
}
