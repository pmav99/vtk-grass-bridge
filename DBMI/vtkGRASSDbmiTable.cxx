
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

#include "vtkGRASSDbmiTable.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>


vtkCxxRevisionMacro(vtkGRASSDbmiTable, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSDbmiTable);

//----------------------------------------------------------------------------

vtkGRASSDbmiTable::vtkGRASSDbmiTable() {
    this->table = db_alloc_table(0);
}

//----------------------------------------------------------------------------

vtkGRASSDbmiTable::~vtkGRASSDbmiTable() {
    if(this->table)
        db_free_table(this->table);
    this->table = NULL;
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiTable::DeepCopy(dbTable *table) {

    if (this->table)
        db_free_table(this->table);

    this->table = db_clone_table(table);

    return;
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiTable::DeepCopy(vtkGRASSDbmiTable *table) {

    this->DeepCopy(table->GetPointer());
    return;
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiTable::SetColumn(vtkGRASSDbmiColumn *column, int idx)
{
    db_set_table_column(this->table, idx, column->GetPointer());
    return;
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiTable::AppendColumn(vtkGRASSDbmiColumn *column) {

    db_append_table_column(this->table, column->GetPointer());
    return;
}

//----------------------------------------------------------------------------
vtkGRASSDbmiColumn *vtkGRASSDbmiTable::GetColumn(int idx)
{
    dbColumn *c = NULL;
    c = db_get_table_column(this->table, idx);
    if(c == NULL)
        return NULL;
    
    VGB_CREATE(vtkGRASSDbmiColumn, column);
    column->DeepCopy(c);
    return column;
}

//----------------------------------------------------------------------------
vtkGRASSDbmiColumn *vtkGRASSDbmiTable::GetColumn(const char* name)
{
    dbColumn *c = NULL;
    int i, columns = this->GetNumberOfColumns();

    for(i = 0; i < columns; i++ ) {
        c = db_get_table_column(this->table, i);
        
        if(c == NULL)
            return NULL;

        if(strcmp(name, db_get_string(&c->columnName)) == 0)
            break;

        c = NULL;
    }
 
    if(c == NULL)
        return NULL;


    VGB_CREATE(vtkGRASSDbmiColumn, column);
    column->DeepCopy(c);
    return column;
}

//----------------------------------------------------------------------------
void
vtkGRASSDbmiTable::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
    os << indent << "Table name:  " << this->GetTableName() << "\n";
    os << indent << "Table description:  " << this->GetTableDescription() << "\n";
    os << indent << "Table number of columns:  " << this->GetNumberOfColumns() << "\n";
    int i;
    for (i = 0; i < this->GetNumberOfColumns(); i++) {
        //this->GetColumn(i)->PrintSelf(os, indent.GetNextIndent());
    }
}

