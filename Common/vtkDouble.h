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

/**
 * \brief This class should be used when a double value is a mutable 
 *        argument of a method and other approaches are not available
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkDouble_h
#define __vtkDouble_h

#include "vtkObject.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkDouble : public vtkObject
{
public:
  static  vtkDouble *New();
  vtkTypeRevisionMacro(vtkDouble,vtkObject);

  vtkGetMacro(Value, double);
  vtkSetMacro(Value, double);

  double Value;
  
protected:
  vtkDouble(){;}
  ~vtkDouble() {};

private:
  vtkDouble(const vtkDouble&);  // Not implemented.
  void operator=(const vtkDouble&);  // Not implemented.
};

#endif
