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
#include "vtkGRASSVectorPolyDataWriter.h"

#include <vtkCellArray.h>
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
#include <vtkInformationVector.h>
#include <vtkInformation.h>

vtkCxxRevisionMacro(vtkGRASSVectorPolyDataWriter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkGRASSVectorPolyDataWriter);

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataWriter::vtkGRASSVectorPolyDataWriter()
{
    this->Mapset = NULL;
    this->VectorName = NULL;
    this->CategoryArrayName = NULL;
    this->SetCategoryArrayName("cats");
    this->SetNumberOfInputPorts(1);
    this->SetNumberOfOutputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataWriter::~vtkGRASSVectorPolyDataWriter()
{
    if (this->VectorName)
        delete [] this->VectorName;
    if (this->Mapset)
        delete [] this->Mapset;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorPolyDataWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
        << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
        << (this->Mapset ? this->Mapset : "(none)") << "\n";

}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
int vtkGRASSVectorPolyDataWriter::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *vtkNotUsed(outputVector))
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);

  // get the input and ouptut
  vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    int i;
    vtkIdType id;

    if (this->VectorName == NULL)
    {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    vtkGRASSVectorMapWriter *writer = vtkGRASSVectorMapWriter::New();


    if (!writer->OpenMap(this->VectorName, 1))
    {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        writer->Delete();
        return -1;
    }

    this->SetMapset(writer->GetMapset());

    vtkGRASSVectorFeatureCats *cats = vtkGRASSVectorFeatureCats::New();
    vtkGRASSVectorFeaturePoints *features = vtkGRASSVectorFeaturePoints::New();

    cats->Delete();
    features->Delete();
    writer->CloseMap();
    writer->Delete();

    return 1;
}
