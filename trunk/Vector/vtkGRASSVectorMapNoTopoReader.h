
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

#ifndef _vtkGRASSVectorMapNoTopoReader_h
#define	_vtkGRASSVectorMapNoTopoReader_h

#include <vtkGRASSVectorMapBase.h>
#include "vtkGRASSBridgeVectorWin32Header.h"

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapNoTopoReader : public vtkGRASSVectorMapBase {
public:
    static vtkGRASSVectorMapNoTopoReader *New();
    vtkTypeRevisionMacro(vtkGRASSVectorMapNoTopoReader, vtkGRASSVectorMapBase);


    /*!\brief Open the vector map read only
     *
     * \param name the name of the vector map
     * \return true or false
     * */
    virtual bool OpenMap(const char *name){return this->OpenMapReadOnly(name);}
    
protected:
    vtkGRASSVectorMapNoTopoReader();
    ~vtkGRASSVectorMapNoTopoReader(){};

private:
    vtkGRASSVectorMapNoTopoReader(const vtkGRASSVectorMapNoTopoReader&); // Not implemented.
    void operator=(const vtkGRASSVectorMapNoTopoReader&); // Not implemented.
};

#endif
