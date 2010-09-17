
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
 * \brief This is the base class for read/write access to vector map database tables
 *
 * This class provides an interface to read grass vector map database tables.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Oct. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSDbmiInterface_h
#define	_vtkGRASSDbmiInterface_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSDefines.h"

class vtkIntArray;
class vtkDataArray;
class vtkGRASSDbmiValue;
class vtkGRASSDbmiCatValueArray;

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiInterface : public vtkObjectGRASSErrorHandler {
public:
    //BTX
    friend class vtkGRASSVectorMapNoTopoReader;
    //ETX

    static vtkGRASSDbmiInterface *New();
    vtkTypeRevisionMacro(vtkGRASSDbmiInterface, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    //!\brief Open a connection to the database. This method must be implemented
    //!in the subclasses
    virtual bool ConnectDB(){return false;}
    //!\brief Close the database connection
    virtual void DisconnectDB();
    //!\brief Select a value from the vector map table. This works only when the
    //!database connection is established.
    virtual bool SelectValue(int cat, const char *column, vtkGRASSDbmiValue *value);
    //!Get the number of rows of the entire table
    virtual int GetNumberOfRows();

    virtual bool SelectCatValueArray(const char *column, const char *where, vtkGRASSDbmiCatValueArray *catval);

    virtual bool SelectCatValueArray(const char *column, vtkGRASSDbmiCatValueArray *catval){return this->SelectCatValueArray(column, NULL, catval);}

    //!\brief Which field number should be used to access the vector database
    vtkSetMacro(FieldNumber, int);
    //!\brief Which field number should be used to access the vector database
    vtkGetMacro(FieldNumber, int);

    //!\brief Get the vector map from which the database connection is based on
    vtkGetObjectMacro(VectorMap, vtkGRASSVectorMapBase);

    //!\brief Check if the database connection is established
    bool IsConnected(){return this->Connected;}

protected:
    vtkGRASSDbmiInterface();
    ~vtkGRASSDbmiInterface();

    vtkSetObjectMacro(VectorMap, vtkGRASSVectorMapBase);

    int FieldNumber;
    bool Connected;
    //BTX
    dbDriver *driver;
    //ETX

    //The vector map which the database connection is based on
    vtkGRASSVectorMapBase *VectorMap;

private:
    vtkGRASSDbmiInterface(const vtkGRASSDbmiInterface&); // Not implemented.
    void operator=(const vtkGRASSDbmiInterface&); // Not implemented.
};

#endif
