
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

    //!\brief Return the number of columns of this table
    int GetNumberOfColumns(){
        if(this->table)
            return db_get_table_number_of_columns(this->table);
        return -1;
    }

    //!\brief Set the name of the table
    //! return 0 if success and 1 in case of failure
    int SetTableName(const char *name) {
        if(this->table)
            return db_set_table_name(this->table, name);
        return DB_FAILED;
    }

    //!\brief Get the name of the table
    //! return NULL in case of failure
    const char *GetTableName(){
        if(this->table)
            return db_get_table_name(this->table);
        return NULL;
    }

    //!\brief Set the description of the table
    //! return 0 if success and 1 in case of failure
    int SetTableDescription(const char *name) {
        if(this->table)
            return db_set_table_name(this->table, name);
        return DB_FAILED;
    }

    //!\brief Get the description of the table
    //! return NULL in case of failure
    const char *GetTableDescription(){
        if(this->table)
            return db_get_table_description(this->table);
        return NULL;
    }

    //!\brief Reses the table to an initial state
    void Reset() {
        if(this->table)
            db_init_table(this->table);
    }
    
    //!\brief Reset the existing and allocate a specific number of columns
    void Allocate(int ncols) {
        if(this->table) 
            db_free_table(this->table);
        this->table = db_alloc_table(ncols);
    }

  //BTX
  virtual dbTable * GetPointer(){return this->table;}
  //ETX

protected:
    vtkGRASSVectorDBTable();
    ~vtkGRASSVectorDBTable();

    //BTX
    dbTable *table;
    void DeepCopyDBTable(dbTable *table);
    //ETX


private:
    vtkGRASSVectorDBTable(const vtkGRASSVectorDBTable&); // Not implemented.
    void operator=(const vtkGRASSVectorDBTable&); // Not implemented.
};

#endif
