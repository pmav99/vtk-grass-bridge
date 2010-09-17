
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

#include "vtkGRASSDbmiInterfaceReader.h"
#include "vtkGRASSDbmiTable.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>


vtkCxxRevisionMacro(vtkGRASSDbmiInterfaceReader, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSDbmiInterfaceReader);

bool vtkGRASSDbmiInterfaceReader::ConnectDB()
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->VectorMap->GetNumberOfDBLinks()) {
        struct field_info *Fi = NULL;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi)
            this->driver = db_start_driver_open_database(Fi->driver, Fi->database);
        else
            return false;
        return this->Connected = true;
    }
    return this->Connected = false;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterfaceReader::GetTable(vtkGRASSDbmiTable *table)
{
    dbTable *t;
    dbString table_name;
    struct field_info *Fi = NULL;

    db_init_string(&table_name);

    if(this->VectorMap && this->VectorMap->IsOpen() && this->VectorMap->GetNumberOfDBLinks()) {

        if(table == NULL)
            return false;

        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi == NULL)
	    return false;
          
	db_set_string(&table_name, Fi->table);
        db_describe_table(this->driver, &table_name, &t);
        table->DeepCopy(t);
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiInterfaceReader::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
}

