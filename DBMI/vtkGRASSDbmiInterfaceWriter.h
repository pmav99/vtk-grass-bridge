
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

#ifndef _vtkGRASSDbmiInterfaceWriter_h
#define	_vtkGRASSDbmiInterfaceWriter_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSDbmiInterface.h"

class vtkGRASSDbmiTable;

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiInterfaceWriter : public vtkGRASSDbmiInterface {
public:

    //BTX
    friend class vtkGRASSVectorMapUpdater;
    friend class vtkGRASSVectorMapWriter;
    //ETX

    static vtkGRASSDbmiInterfaceWriter *New();
    vtkTypeRevisionMacro(vtkGRASSDbmiInterfaceWriter, vtkGRASSDbmiInterface);

    //!\brief Open a connection to the database. A databse connection can only
    //!be established when a VectorMap was specified.
    virtual bool ConnectDB(){;}

    //!\brief Open a connection to the database. A databse connection can only
    //!be established when a VectorMap was specified.
    virtual bool ConnectDBCreateTable(vtkGRASSDbmiTable *table){;}

    //!\brief Get the vector table 
    virtual bool GetTable(vtkGRASSDbmiTable *table){;}

protected:
    vtkGRASSDbmiInterfaceWriter(){;}
    ~vtkGRASSDbmiInterfaceWriter(){;}

private:
    vtkGRASSDbmiInterfaceWriter(const vtkGRASSDbmiInterfaceWriter&); // Not implemented.
    void operator=(const vtkGRASSDbmiInterfaceWriter&); // Not implemented.
};

#endif
