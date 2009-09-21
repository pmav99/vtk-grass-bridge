
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

#include "vtkGRASSVectorMapTopoReader.h"

#include "vtkGRASSVectorBBox.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSVectorFeaturePoints.h>
#include <vtkGRASSVectorFeatureCats.h>


vtkCxxRevisionMacro(vtkGRASSVectorMapTopoReader, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorMapTopoReader);

//----------------------------------------------------------------------------

vtkGRASSVectorMapTopoReader::vtkGRASSVectorMapTopoReader()
{
    this->SetVectorLevelToTopo();
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapTopoReader::ReadFeature(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats, int index)
{
    char buff[1024];
    int ret = 1;

    ret = this->CheckBeforOpen(points, cats);
    if(ret != 1)
        return ret;

    ret = Vect_read_line(&this->map, points->GetPointer(), cats->GetPointer(), index);

    if (ret == -1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Error wihle reading vector map <%s>.",
                   this->GetClassName(), __LINE__, this->GetFullName());
        this->InsertNextError(buff);
        return ret;
    }

    if(ret >= 0)
        points->SetFeatureType(ret);

    return ret;
}

//----------------------------------------------------------------------------

void vtkGRASSVectorMapTopoReader::GetBoundingBox(vtkGRASSVectorBBox *box)
{

    if(box == NULL)
        return;
    Vect_get_map_box(&this->map, box->GetPointer());
    return;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorMapTopoReader::PrintSelf(ostream& os, vtkIndent indent)
{

    this->Superclass::PrintSelf(os, indent);

    if(this->Open){
        indent = indent.GetNextIndent();
        os << indent << "Number of features: " << this->GetNumberOfFeatures() << endl;
        os << indent << "Number of nodes: " << this->GetNumberOfNodes() << endl;
        os << indent << "Number of points: " << this->GetNumberOfPoints() << endl;
        os << indent << "Number of lines: " << this->GetNumberOfLines() << endl;
        os << indent << "Number of boundaries: " << this->GetNumberOfBoundaries() << endl;
        os << indent << "Number of areas: " << this->GetNumberOfAreas() << endl;
        os << indent << "Number of centroids: " << this->GetNumberOfCentroids() << endl;
        os << indent << "Number of isles: " << this->GetNumberOfIsles() << endl;
        os << indent << "Number of faces: " << this->GetNumberOfFaces() << endl;
        os << indent << "Number of kernels: " << this->GetNumberOfKernels() << endl;
        os << indent << "Number of db-links: " << this->GetNumberOfDBLinks() << endl;
    }
}



