
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
#include <vtkIntArray.h>


vtkCxxRevisionMacro(vtkGRASSVectorMapTopoReader, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorMapTopoReader);

//----------------------------------------------------------------------------

vtkGRASSVectorMapTopoReader::vtkGRASSVectorMapTopoReader()
{
    this->SetVectorLevelToTopo();
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapTopoReader::ReadFeature( int index, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats)
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

int vtkGRASSVectorMapTopoReader::GetAreaFromCentroid(int centroid)
{
    if(this->Open){
        return Vect_get_centroid_area(&this->map, centroid);
    }
    return -1;
}

//----------------------------------------------------------------------------

int vtkGRASSVectorMapTopoReader::GetArea( int area, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats)
{
    int ret = -1;
    if(this->Open){
        Vect_get_area_cats(&this->map, area, cats->GetPointer());
        ret =  Vect_get_area_points (&this->map, area, points->GetPointer());
        points->SetFeatureTypeToArea();
        return ret;
    }
    return ret;
}

//----------------------------------------------------------------------------

int vtkGRASSVectorMapTopoReader::GetAreaBoundaries(int area, vtkIntArray* boundaryids)
{
    ilist *ids = Vect_new_list();
    int ret = -1;
    int i;

    boundaryids->Initialize();

    if(this->Open){
        ret = Vect_get_area_boundaries(&this->map, area, ids);

        for(i = 0; i < ids->n_values; i++)
        {
            boundaryids->InsertNextValue(ids->value[i]);
        }
    }

    Vect_destroy_list(ids);
    return ret;
}

//----------------------------------------------------------------------------

int vtkGRASSVectorMapTopoReader::GetIsleBoundaries(int isle, vtkIntArray* boundaryids)
{
    ilist *ids = Vect_new_list();
    int ret = -1;
    int i;

    boundaryids->Initialize();

    if(this->Open){
        ret = Vect_get_isle_boundaries(&this->map, isle, ids);

        for(i = 0; i < ids->n_values; i++)
        {
            boundaryids->InsertNextValue(ids->value[i]);
        }
    }

    Vect_destroy_list(ids);
    return ret;
}

//------------------------------------------------------------------------------

int vtkGRASSVectorMapTopoReader::SelectLinesByBox(vtkGRASSVectorBBox *box, int type, vtkIntArray *ids)
{
    ilist *pids = Vect_new_list();
    int ret = -1;
    int i;

    ids->Initialize();

    if(this->Open){
        ret = Vect_select_lines_by_box(&this->map, box->GetPointer(), type, pids);

        for(i = 0; i < pids->n_values; i++)
        {
            ids->InsertNextValue(pids->value[i]);
        }
    }

    Vect_destroy_list(pids);
    return ret;
}

//------------------------------------------------------------------------------

int vtkGRASSVectorMapTopoReader::SelectAreasByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids)
{
    ilist *pids = Vect_new_list();
    int ret = -1;
    int i;

    ids->Initialize();

    if(this->Open){
        ret = Vect_select_areas_by_box(&this->map, box->GetPointer(), pids);

        for(i = 0; i < pids->n_values; i++)
        {
            ids->InsertNextValue(pids->value[i]);
        }
    }

    Vect_destroy_list(pids);
    return ret;
}

//------------------------------------------------------------------------------

int vtkGRASSVectorMapTopoReader::SelectIslesByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids)
{
    ilist *pids = Vect_new_list();
    int ret = -1;
    int i;

    ids->Initialize();

    if(this->Open){
        ret = Vect_select_isles_by_box(&this->map, box->GetPointer(), pids);

        for(i = 0; i < pids->n_values; i++)
        {
            ids->InsertNextValue(pids->value[i]);
        }
    }

    Vect_destroy_list(pids);
    return ret;
}

//------------------------------------------------------------------------------

int vtkGRASSVectorMapTopoReader::SelectNodesByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids){
    ilist *pids = Vect_new_list();
    int ret = -1;
    int i;

    ids->Initialize();

    if(this->Open){
        ret = Vect_select_nodes_by_box(&this->map, box->GetPointer(), pids);

        for(i = 0; i < pids->n_values; i++)
        {
            ids->InsertNextValue(pids->value[i]);
        }
    }

    Vect_destroy_list(pids);
    return ret;
}

//------------------------------------------------------------------------------

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



