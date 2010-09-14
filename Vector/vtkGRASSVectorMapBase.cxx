
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

#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSVectorBBox.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSVectorFeaturePoints.h>
#include <vtkGRASSVectorFeatureCats.h>
#include <vtkGRASSDefines.h>


vtkCxxRevisionMacro(vtkGRASSVectorMapBase, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorMapBase);

//----------------------------------------------------------------------------

vtkGRASSVectorMapBase::vtkGRASSVectorMapBase()
{
    this->Open = false;
    this->VectorLevel = 0;
    this->SetVectorLevelToNoTopo();
    this->VectorName = NULL;
    this->Initiated = false;
    this->TotalNumberOfPoints = 0;
    //VGB_CREATE(vtkGRASSVectorDBInterface, this->dbAccess);
}

//----------------------------------------------------------------------------

vtkGRASSVectorMapBase::~vtkGRASSVectorMapBase()
{
    this->CloseMap();
    if (this->VectorName)
        delete [] this->VectorName;
}

//----------------------------------------------------------------------------

bool
vtkGRASSVectorMapBase::OpenMapReadOnly(const char *name)
{
    char *mapset = NULL;
    char buff[1024];

    // Check if the same map is already opened
    if (this->Open == true && strcmp(name, this->VectorName) == 0)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Vector map %s is already open.",
                   this->GetClassName(), __LINE__, this->VectorName);
        this->InsertNextError(buff);
        return false;
    }
    else if (this->Open == true)
    {
        // If a new name is given, the open map will be closed
        this->CloseMap();
    }

    this->SetVectorName(name);

    Vect_set_open_level(this->VectorLevel);

    if (!setjmp(vgb_stack_buffer))
    {
        if (1 > Vect_open_old(&this->map, name, mapset))
        {
            G_snprintf(buff, 1024, "class: %s line: %i Unable to open vector map <%s>.",
                       this->GetClassName(), __LINE__, name);
            this->InsertNextError(buff);
            return false;
        }
    }
    else
    {
        this->InsertNextError(vgb_error_message);
        this->Open = false;
        return false;
    }
    this->RewindMap();

    if (mapset)
        G_free(mapset);

    this->Open = true;

    //this->dbAccess->SetFieldNumber(1);
    //this->dbAccess->InitializeDB(this);

    return true;
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapBase::CheckBeforOpen(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats)
{
    char buff[1024];

    if (this->Open == false)
    {
        G_snprintf(buff, 1024, "class: %s line: %i vector map was not opened.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return -5;
    }
    if (points == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i points must be initialised.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return -3;
    }
    if (cats == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i cats must be initialised.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return -3;
    }

    return 1;
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapBase::ReadNextFeature(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats)
{
    char buff[1024];
    int ret = 1;

    ret = this->CheckBeforOpen(points, cats);
    if (ret != 1)
        return ret;

    points->Reset();
    cats->Reset();

    TRY ret = Vect_read_next_line(&this->map, points->GetPointer(), cats->GetPointer()); CATCH_INT


    if (ret == -1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Error wihle reading vector map <%s>.",
                   this->GetClassName(), __LINE__, this->GetFullName());
        this->InsertNextError(buff);
        return ret;
    }

    if (ret >= 0)
        points->SetFeatureType(ret);

    return ret;
}

//----------------------------------------------------------------------------

bool
vtkGRASSVectorMapBase::CloseMap()
{
    char buff[1024];

    if (this->Open == false)
        return true;


    Vect_set_release_support(&this->map);

    if (!setjmp(vgb_stack_buffer))
    {
        if (Vect_close(&this->map) != 0)
        {
            G_snprintf(buff, 1024, "class: %s line: %i Error while closing vector map <%s>.",
                       this->GetClassName(), __LINE__, this->GetFullName());
            this->InsertNextError(buff);
            this->Open = false;
            return false;
        }
    }
    else
    {
        this->InsertNextError(vgb_error_message);
        this->Open = false;
        return false;
    }

    this->Open = false;
    this->Initiated = false;
    this->TotalNumberOfPoints = 0;

    //this->dbAccess->ShutdownDB();

    return true;
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapBase::GetTotalNumberOfPoints()
{
    if (this->Open == false)
        return 0;
    if (this->Initiated == true)
        return this->TotalNumberOfPoints;

    this->RewindMap();

    VGB_CREATE(vtkGRASSVectorFeaturePoints, points);
    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);

    if (!setjmp(vgb_stack_buffer))
    {
        while (0 < this->ReadNextFeature(points, cats))
        {
            this->TotalNumberOfPoints += points->GetNumberOfPoints();
        }
    }
    else
    {
        points->Delete();
        cats->Delete();
        return -1;
    }

    this->RewindMap();

    this->Initiated = true;

    return this->TotalNumberOfPoints;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorMapBase::PrintSelf(ostream& os, vtkIndent indent)
{

    this->Superclass::PrintSelf(os, indent);

    os << indent << "Is Open: " << (this->Open ? "True:" : "False") << endl;

    if (this->Open)
    {
        indent = indent.GetNextIndent();
        os << indent << "Organisation: " << this->GetOrganisation() << endl;
        os << indent << "CreationDate: " << this->GetCreationDate() << endl;
        os << indent << "Person: " << this->GetPerson() << endl;
        os << indent << "Title: " << this->GetTitle() << endl;
        os << indent << "FullName: " << this->GetFullName() << endl;
        os << indent << "Mapset: " << this->GetMapset() << endl;
        os << indent << "MapDate: " << this->GetMapDate() << endl;
        os << indent << "Scale: " << this->GetScale() << endl;
        os << indent << "Comment: " << this->GetComment() << endl;
        os << indent << "Zone: " << this->GetZone() << endl;
        os << indent << "Threshold: " << this->GetThreshold() << endl;
        os << indent << "Projection: " << this->GetProjection() << endl;
        os << indent << "ProjectionName: " << this->GetProjectionName() << endl;
        os << indent << "Is 3d: " << (this->Is3d() ? "Yes" : "No") << endl;
        os << indent << "Total number of coor points: " << this->GetTotalNumberOfPoints() << endl;
    }
}



