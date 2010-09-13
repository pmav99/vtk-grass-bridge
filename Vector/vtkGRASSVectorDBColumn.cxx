
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

#include "vtkGRASSVectorDBColumn.h"
#include "vtkGRASSRasterMapBase.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>


vtkCxxRevisionMacro(vtkGRASSVectorDBColumn, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorDBColumn);

//----------------------------------------------------------------------------

vtkGRASSVectorDBColumn::vtkGRASSVectorDBColumn()
{
    this->column = (dbColumn *) db_calloc(sizeof(dbColumn), ncols);
    db_init_column(this->column);
}

//----------------------------------------------------------------------------

vtkGRASSVectorDBColumn::~vtkGRASSVectorDBColumn()
{

    if (this->column)
        db_free_column(this->column);
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorDBColumn::Initialize()
{

    if (this->column)
        db_free_column(this->column);

    this->column = (dbColumn *) db_calloc(sizeof(dbColumn), ncols);
    db_init_column(this->column);
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorDBColumn::DeepCopyDBColumn(dbColumn *column)
{

    if (this->column == column)
        return;

    this->Initialize();
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorDBColumn::PrintSelf(ostream& os, vtkIndent indent)
{

    this->Superclass::PrintSelf(os, indent);
    if (this->column)
    {
        os << indent << "Name: " << this->GetName() << endl;
        os << indent << "Description: " << this->GetDescription() << endl;
        os << indent << "HostType: " << this->GetHostType() << endl;
        os << indent << "Length: " << this->GetLength() << endl;
        os << indent << "SQLType: " << this->GetSQLType() << endl;
        os << indent << "Precision: " << this->GetPrecision() << endl;
        os << indent << "Scale: " << this->GetScale() << endl;
        os << indent << "SelectPrivileg: " << this->GetSelectPrivileg() << endl;
        os << indent << "ValueAsString: " << this->GetValueAsString() << endl;
        os << indent << "UpdatePrivileg: " << this->GetUpdatePrivileg() << endl;
    }
}

