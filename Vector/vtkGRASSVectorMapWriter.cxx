
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

#include "vtkGRASSVectorMapWriter.h"
#include "vtkGRASSDefines.h"
#include <vtkObjectFactory.h>


vtkCxxRevisionMacro(vtkGRASSVectorMapWriter, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorMapWriter);

//----------------------------------------------------------------------------

vtkGRASSVectorMapWriter::vtkGRASSVectorMapWriter()
{
    this->SetVectorLevelToTopo();
}

int
vtkGRASSVectorMapWriter::WriteFeature(int type, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats)
{
    if (this->Open)
    {
        if (!setjmp(vgb_stack_buffer))
        {
            return Vect_write_line(&this->map, type, points->GetPointer(), cats->GetPointer());
        }
        else
        {
            this->InsertNextError(vgb_error_message);
            return -1;
        }
    }
    else return -1;
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapWriter::DeleteFeature(int feature)
{
    if (this->Open)
    {
        if (!setjmp(vgb_stack_buffer))
        {
            return Vect_delete_line(&this->map, feature);
        }
        else
        {
            this->InsertNextError(vgb_error_message);
            return -1;
        }
    }
    else return -1;
}

//----------------------------------------------------------------------------

bool
vtkGRASSVectorMapWriter::OpenMap(const char *name, int with_z)
{
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
        if (1 > Vect_open_new(&this->map, name, with_z))
        {
            this->InsertNextError(vgb_error_message);
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

    this->Open = true;
    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSVectorMapWriter::CloseMap(int build_topo)
{
    char buff[1024];

    if (this->Open == false)
        return true;

    if (build_topo == 1 && Vect_build(&this->map) != 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Error while closing vector map <%s>.",
                   this->GetClassName(), __LINE__, this->GetFullName());
        this->InsertNextError(buff);
        this->Open = false;
        return false;
    }

    Vect_set_category_index_update(&this->map);
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
    return true;
}
