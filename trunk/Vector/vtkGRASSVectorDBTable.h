
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

/*!
 * \brief This is the base class for read/write access to a vector map database table
 *
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Oct. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSVectorDBTable_h
#define	_vtkGRASSVectorDBTable_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSVectorDBColumn.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorDBTable : public vtkObjectGRASSErrorHandler {
public:

    //BTX
    friend class vtkGRASSVectorDBInterface;
    //ETX

    static vtkGRASSVectorDBTable *New();
    vtkTypeRevisionMacro(vtkGRASSVectorDBTable, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    int GetNumberOfRows(){
        if(this->cursor)
            db_get_num_rows(this->cursor);
        else
            return -1;
    }

    int GetNumberOfColumns(){
        if(this->table)
            return db_get_table_number_of_columns(this->table);
        else
            return -1;
    }
    
protected:
    vtkGRASSVectorDBTable();
    ~vtkGRASSVectorDBTable();

    //!\brief Initialize the database, to be called by the VectorMap reader/writer
    virtual bool InitializeTable(dbTable *table, dbDriver *driver, dbHandle *handle, dbCursor *cursor);

    //BTX
    dbDriver *driver;
    dbHandle *handle;
    dbCursor *cursor;
    dbTable *table;
    //ETX

    vtkGRASSVectorDBColumn *Column;


private:
    vtkGRASSVectorDBTable(const vtkGRASSVectorDBTable&); // Not implemented.
    void operator=(const vtkGRASSVectorDBTable&); // Not implemented.
};

#endif
