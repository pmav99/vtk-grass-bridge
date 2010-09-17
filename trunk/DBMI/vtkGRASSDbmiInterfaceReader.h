
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

#ifndef _vtkGRASSDbmiInterfaceReader_h
#define	_vtkGRASSDbmiInterfaceReader_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSDbmiInterface.h"
#include "vtkGRASSDbmiCatValueArray.h"

class vtkIntArray;
class vtkDataArray;
class vtkGRASSDbmiCatValueArray;
class vtkGRASSDbmiTable;

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiInterfaceReader : public vtkGRASSDbmiInterface {
public:

    //BTX
    friend class vtkGRASSVectorMapNoTopoReader;
    friend class vtkGRASSVectorMapTopoReader;
    //ETX

    static vtkGRASSDbmiInterfaceReader *New();
    vtkTypeRevisionMacro(vtkGRASSDbmiInterfaceReader, vtkGRASSDbmiInterface);
    void PrintSelf(ostream& os, vtkIndent indent);

    //!\brief Open a connection to the database. A databse connection can only
    //!be established when a VectorMap was specified.
    virtual bool ConnectDB();

    //!\brief Get the vector table 
    virtual bool GetTable(vtkGRASSDbmiTable *table);

protected:
    vtkGRASSDbmiInterfaceReader(){;}
    ~vtkGRASSDbmiInterfaceReader(){;}

private:
    vtkGRASSDbmiInterfaceReader(const vtkGRASSDbmiInterfaceReader&); // Not implemented.
    void operator=(const vtkGRASSDbmiInterfaceReader&); // Not implemented.
};

#endif
