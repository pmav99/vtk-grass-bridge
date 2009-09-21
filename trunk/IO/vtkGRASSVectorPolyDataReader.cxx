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
#include "vtkGRASSVectorPolyDataReader.h"

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


vtkCxxRevisionMacro(vtkGRASSVectorPolyDataReader, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkGRASSVectorPolyDataReader);

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataReader::vtkGRASSVectorPolyDataReader()
{
    this->Mapset = NULL;
    this->VectorName = NULL;
    this->SetNumberOfInputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataReader::~vtkGRASSVectorPolyDataReader()
{
    if (this->VectorName)
        delete [] this->VectorName;
    if (this->Mapset)
        delete [] this->Mapset;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorPolyDataReader::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
        << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
        << (this->Mapset ? this->Mapset : "(none)") << "\n";

}

//----------------------------------------------------------------------------

int
vtkGRASSVectorPolyDataReader::RequestData(vtkInformation*,
                                          vtkInformationVector**,
                                          vtkInformationVector* outputVector)
{
    // Allocate objects to hold points and vertex cells.
    vtkPoints *points = vtkPoints::New();

    if (this->VectorName == NULL)
    {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    vtkGRASSVectorMapNoTopoReader *reader = vtkGRASSVectorMapNoTopoReader::New();
    vtkGRASSVectorFeatureCats *cats = vtkGRASSVectorFeatureCats::New();
    vtkGRASSVectorFeaturePoints *features = vtkGRASSVectorFeaturePoints::New();


    if (!reader->OpenMap(this->VectorName))
    {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        reader->Delete();
        cats->Delete();
        features->Delete();
        return -1;
    }

    this->SetMapset(reader->GetMapset());

    vtkPolyData* output = vtkPolyData::GetData(outputVector);
    output->Allocate(1);

    vtkIntArray *categories = vtkIntArray::New();
    categories->SetNumberOfComponents(1);
    categories->SetName("cat");

    vtkIdList *ids = vtkIdList::New();
    int i;
    vtkIdType id;

    // Read every feature in vector map
    while (reader->ReadNextFeature(features, cats) > 0)
    {
        for(i = 0; i < features->GetNumberOfPoints(); i++)
        {
            double *point = features->GetPoint(i);
            id = points->InsertNextPoint(point[0], point[1], point[2]);
            ids->InsertNextId(id);
        }
        output->InsertNextCell(features->GetVTKCellId(), ids);
        ids->Initialize();

        categories->InsertNextValue(cats->GetCat(1));
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
    features->Delete();

    return 1;
}
