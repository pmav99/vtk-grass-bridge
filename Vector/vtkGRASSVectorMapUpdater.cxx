
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

#include "vtkGRASSVectorMapUpdater.h"
#include <vtkObjectFactory.h>


vtkCxxRevisionMacro(vtkGRASSVectorMapUpdater, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorMapUpdater);

//----------------------------------------------------------------------------

vtkGRASSVectorMapUpdater::vtkGRASSVectorMapUpdater()
{
    this->SetVectorLevelToTopo();
}


//----------------------------------------------------------------------------

bool
vtkGRASSVectorMapUpdater::OpenMap(const char *name, int with_z)
{
    char buff[1024];

    // Check if the same map is already opened
    if (this->Open == true && strcmp(name, this->VectorName) == 0)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Vector map %s is already open.",
                   this->GetClassName(), __LINE__, this->VectorName);
        this->InsertNextError(buff);
        return false;
    } else if (this->Open == true) {
        // If a new name is given, the open map will be closed
        this->CloseMap();
    }

    this->SetVectorName(name);

    Vect_set_open_level(this->VectorLevel);

    if (1 > Vect_open_update(&this->map, name, ""))
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to open vector map <%s>.",
                   this->GetClassName(), __LINE__, name);
        this->InsertNextError(buff);
        return false;
    }

    this->Open = true;
    return true;
}
