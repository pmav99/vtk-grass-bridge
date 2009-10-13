
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
 * \brief This is the base class for read/write access to a vector map. database tables
 *
 * This class provides an interface to read grass vector map database tables.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Oct. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSVectorDBInterface_h
#define	_vtkGRASSVectorDBInterface_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSDefines.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorDBInterface : public vtkObjectGRASSErrorHandler {
public:

    //BTX
    friend class vtkGRASSVectorMapBase;
    //ETX

    static vtkGRASSVectorDBInterface *New();
    vtkTypeRevisionMacro(vtkGRASSVectorDBInterface, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    virtual bool StartDriver();

    virtual const char *GetDatabaseName(){
        if(this->fieldInfo)
        return this->fieldInfo->database;
        else return NULL;
    }

    virtual const char *GetDriverName(){
        if(this->fieldInfo)
        return this->fieldInfo->driver;
        else return NULL;
    }

    virtual const char *GetKeyName(){
        if(this->fieldInfo)
        return this->fieldInfo->key;
        else return NULL;
    }

    virtual const char *GetName(){
        if(this->fieldInfo)
        return this->fieldInfo->name;
        else return NULL;
    }
    virtual const char *GetTableName(){
        if(this->fieldInfo)
        return this->fieldInfo->table;
        else return NULL;
    }

    virtual bool IsInitialized() {
        return this->Initialized;
    }

    vtkSetMacro(FieldNumber, int);
    vtkGetMacro(FieldNumber, int);
    vtkGetMacro(NumberOfColumns, int);


protected:
    vtkGRASSVectorDBInterface();
    ~vtkGRASSVectorDBInterface();

    vtkSetObjectMacro(VectorMap, vtkGRASSVectorMapBase);
    vtkGetObjectMacro(VectorMap, vtkGRASSVectorMapBase);

    //!\brief Initialize the database, to be called by the VectorMap reader/writer
    virtual bool InitializeDB(vtkGRASSVectorMapBase *VectorMap);
    virtual bool ShutdownDB();

    int FieldNumber;
    int NumberOfColumns;

    //BTX
    dbDriver *driver;
    dbHandle handle;
    dbCursor cursor;
    dbTable *table;
    dbColumn *column;
    dbString table_name;
    dbString dbsql;
    dbString valstr;
    struct field_info *fieldInfo;
    //ETX

    bool Initialized;

    vtkGRASSVectorMapBase *VectorMap;

private:
    vtkGRASSVectorDBInterface(const vtkGRASSVectorDBInterface&); // Not implemented.
    void operator=(const vtkGRASSVectorDBInterface&); // Not implemented.
};

#endif
