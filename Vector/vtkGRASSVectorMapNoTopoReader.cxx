
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

#include "vtkGRASSVectorMapNoTopoReader.h"
#include "vtkGRASSDbmiInterfaceReader.h"
#include <vtkObjectFactory.h>


vtkCxxRevisionMacro(vtkGRASSVectorMapNoTopoReader, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorMapNoTopoReader);

//----------------------------------------------------------------------------

vtkGRASSVectorMapNoTopoReader::vtkGRASSVectorMapNoTopoReader()
{
    this->DbmiInterface = NULL;
    this->SetVectorLevelToNoTopo();
}

//----------------------------------------------------------------------------

vtkGRASSVectorMapNoTopoReader::~vtkGRASSVectorMapNoTopoReader()
{
    if(this->DbmiInterface)
        this->DbmiInterface->Delete();
}

//----------------------------------------------------------------------------

bool vtkGRASSVectorMapNoTopoReader::OpenMap(const char *name)
{
    bool state;
    state = this->OpenMapReadOnly(name);
    if(state)
    {
        this->DbmiInterface = vtkGRASSDbmiInterfaceReader::New();
        this->DbmiInterface->SetVectorMap(this);
    }
    return state;
}