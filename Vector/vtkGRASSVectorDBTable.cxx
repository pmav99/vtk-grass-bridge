
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

#include "vtkGRASSVectorDBTable.h"
#include "vtkGRASSRasterMapBase.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>


vtkCxxRevisionMacro(vtkGRASSVectorDBTable, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorDBTable);

//----------------------------------------------------------------------------

vtkGRASSVectorDBTable::vtkGRASSVectorDBTable() {
    this->table = NULL;
}

//----------------------------------------------------------------------------

vtkGRASSVectorDBTable::~vtkGRASSVectorDBTable() {
    ;
}

void
vtkGRASSVectorDBTable::DeepCopyDBTable(dbTable *table) {

    
    return;
}


void
vtkGRASSVectorDBTable::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
    os << indent << "Table name:  " << this->GetTableName() << "\n";
    os << indent << "Table description:  " << this->GetTableDescription() << "\n";
    int i;
    for (i = 0; i < this->GetNumberOfColumns(); i++)
    {
        ;
    }
}

