
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

#include "vtkGRASSVectorDBInterface.h"
#include "vtkGRASSRasterMapBase.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>


vtkCxxRevisionMacro(vtkGRASSVectorDBInterface, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSVectorDBInterface);

//----------------------------------------------------------------------------

vtkGRASSVectorDBInterface::vtkGRASSVectorDBInterface() {
    this->Initialized = false;
    this->driver = NULL;
    this->column = NULL;
    this->table = NULL;
    this->FieldNumber = 1;

    db_init_string(&this->dbsql);
    db_init_string(&this->valstr);
    db_init_string(&this->table_name);
    db_init_handle(&this->handle);

    this->VectorMap = NULL;
}

//----------------------------------------------------------------------------

vtkGRASSVectorDBInterface::~vtkGRASSVectorDBInterface() {
    ;
}

//----------------------------------------------------------------------------

bool vtkGRASSVectorDBInterface::InitializeDB(vtkGRASSVectorMapBase *VectorMap) {
    TRY
    if (VectorMap) {
        this->SetVectorMap(VectorMap);
        this->Initialized = true;

        this->fieldInfo = Vect_get_field((const Map_info*)this->VectorMap, this->FieldNumber);

        /* Prepearing database for use */
        this->driver = db_start_driver(this->fieldInfo->driver);
        if (this->driver == NULL) {
            G_fatal_error(("Unable to start driver <%s>"), this->fieldInfo->driver);
        }
        db_set_handle(&this->handle, this->fieldInfo->database, NULL);
        if (db_open_database(this->driver, &this->handle) != DB_OK) {
            G_fatal_error(("Unable to open database <%s> by driver <%s>"),
                    this->fieldInfo->database, driver);
        }
        db_set_string(&this->table_name, this->fieldInfo->table);
        if (db_describe_table(this->driver, &this->table_name, &this->table) != DB_OK) {
            G_fatal_error(("Unable to describe table <%s>"), this->fieldInfo->table);
        }
        this->NumberOfColumns = db_get_table_number_of_columns(this->table);

    } else
        return false;
    CATCH_BOOL;
}

//----------------------------------------------------------------------------

bool vtkGRASSVectorDBInterface::ShutdownDB() {
    if (this->driver) {
        if (db_close_database_shutdown_driver(this->driver))
            return true;
        else
            return false;
    } else
        return false;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorDBInterface::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);


    if (this->VectorMap)
        this->VectorMap->PrintSelf(os, indent);
}

