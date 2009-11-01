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

vtkCxxRevisionMacro(vtkGRASSVectorPolyDataWriter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkGRASSVectorPolyDataWriter);

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataWriter::vtkGRASSVectorPolyDataWriter()
{
    this->Mapset = NULL;
    this->VectorName = NULL;
    this->CategoryArrayName = NULL;
    this->BuildTopoOff();
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
    if (this->CategoryArrayName)
        delete [] this->CategoryArrayName;
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

int
vtkGRASSVectorPolyDataWriter::RequestData(
                                          vtkInformation *vtkNotUsed(request),
                                          vtkInformationVector **inputVector,
                                          vtkInformationVector *vtkNotUsed(outputVector))
{
    // get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);

    // get the input and ouptut
    vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    int i, j;
    double point[3], pcoords[3];

    if (this->VectorName == NULL)
    {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    VGB_CREATE(vtkGRASSVectorMapWriter, writer);


    if (!writer->OpenMap(this->VectorName, 1))
    {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        writer->Delete();
        return -1;
    }

    this->SetMapset(writer->GetMapset());

    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, feature);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, centroid);

    // We write the data per cell
    for (i = 0; i < input->GetNumberOfCells(); i++)
    {
        vtkDataArray *categories = NULL;
        if (input->GetCellData()->HasArray(this->CategoryArrayName))
            categories = input->GetCellData()->GetArray(this->CategoryArrayName);

        vtkCell *cell = input->GetCell(i);

        if (cell->GetCellType() == VTK_POLY_VERTEX)
        {
            vtkErrorMacro( << "VTK_POLY_VERTEX is currently not supported.");
            continue;
        }

        // Only linear 2d cells are supported
        if (cell->IsLinear() && cell->GetCellDimension() < 3)
        {
            // Reset the feature and the categorie arrays
            feature->Reset();
            centroid->Reset();
            cats->Reset();

            // Get the point coordiantes
            vtkPoints *points = cell->GetPoints();
            for (j = 0; j < cell->GetNumberOfPoints(); j++)
            {
                points->GetPoint(j, point);
                feature->AppendPoint(point[0], point[1], point[2]);
            }

            if (categories)
                cats->AddCat(1, (int) categories->GetTuple1(i));

            // Only a subset of cells are supported
            if (cell->GetCellType() == VTK_VERTEX)
                feature->SetFeatureTypeToPoint();
            else if (cell->GetCellType() == VTK_TRIANGLE || cell->GetCellType() == VTK_QUAD)
                feature->SetFeatureTypeToFace();
            else if (cell->GetCellType() == VTK_POLY_LINE || cell->GetCellType() == VTK_LINE)
                feature->SetFeatureTypeToLine();
            else if (cell->GetCellType() == VTK_POLYGON)
            {
                double *weights = new double [cell->GetNumberOfPoints()];
                int subId;
                feature->SetFeatureTypeToBoundary();
                centroid->SetFeatureTypeToCentroid();
                // Compute the centroid
                subId = cell->GetParametricCenter(pcoords);
                cell->EvaluateLocation(subId, pcoords, point, weights);
                // Append the centroid
                centroid->AppendPoint(point[0], point[1], point[2]);
                delete [] weights;

                if (1 > writer->WriteFeature(centroid, cats))
                {
                    vtkErrorMacro( << "Error writing centroid to vector map");
                    cats->Delete();
                    feature->Delete();
                    delete [] weights;
                    return -1;
                }
            }
            else
                continue;


            if (1 > writer->WriteFeature(feature, cats))
            {
                vtkErrorMacro( << "Error writing feature to vector map");
                cats->Delete();
                feature->Delete();
                return -1;
            }
        }
        else
        {
            vtkWarningMacro( << "cell type " << cell->GetCellType() << " not supported");
            continue;
        }
    }

    if (this->BuildTopo > 0)
        writer->CloseMap(true);
    else
        writer->CloseMap(false);

    return 1;
}
