
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

#ifndef _vtkGRASSDbmiCatValueArray_h
#define	_vtkGRASSDbmiCatValueArray_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSDefines.h"

extern "C" {
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiCatValueArray : public vtkObjectGRASSErrorHandler {
public:

    static vtkGRASSDbmiCatValueArray *New();
    vtkTypeRevisionMacro(vtkGRASSDbmiCatValueArray, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    void DeepCopy(vtkGRASSDbmiCatValueArray *catval);
  //BTX
  virtual dbTable * GetPointer(){return this->catval;}
    void DeepCopy(dbCatValArray *catval);
  //ETX

protected:
    vtkGRASSDbmiCatValueArray();
    ~vtkGRASSDbmiCatValueArray();

    //BTX
    dbCatValArray *catval;
    //ETX

private:
    vtkGRASSDbmiCatValueArray(const vtkGRASSDbmiCatValueArray&); // Not implemented.
    void operator=(const vtkGRASSDbmiCatValueArray&); // Not implemented.
};

#endif
