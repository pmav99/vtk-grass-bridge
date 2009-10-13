
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

    this->column = NULL;
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

    this->column = (dbColumn*) (calloc(1, sizeof (dbColumn)));
    db_init_column(this->column);
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorDBColumn::SetColumn(dbColumn *column)
{

    if (this->column == column)
        return;

    if (this->column)
        db_free_column(this->column);

    this->column = column;
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
        os << indent << "Name: " << this->GetHostType() << endl;
        os << indent << "Name: " << this->GetLength() << endl;
        os << indent << "Name: " << this->GetSQLType() << endl;
        os << indent << "Name: " << this->GetPrecision() << endl;
        os << indent << "Name: " << this->GetScale() << endl;
        os << indent << "Name: " << this->GetSelectPrivate() << endl;
        os << indent << "Name: " << this->GetValueAsString() << endl;
        os << indent << "Name: " << this->GetUpdatePrivate() << endl;
    }
}

