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
 * \brief
 * 
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkDCELL_h
#define __vtkDCELL_h

#include "vtkObject.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster.h>
}


class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkDCELL : public vtkObject
{
public:
  static  vtkDCELL *New();
  vtkTypeRevisionMacro(vtkDCELL,vtkObject);

  double GetValueAsInt(){return (int)this->Value;}
  double GetValueAsFloat(){return (float)this->Value;}
  double GetValueAsDouble(){return (double)this->Value;}

  void SetDoubleValue(double value){this->Value = (DCELL)value;}

  bool IsNull(){return (Rast_is_d_null_value(&(this->Value))?true:false);}
  void SetNull(){Rast_set_d_null_value(&(this->Value), 1);}

  //BTX
  DCELL Value;
  //ETX

protected:
  vtkDCELL(){;}
  ~vtkDCELL() {};

private:
  vtkDCELL(const vtkDCELL&);  // Not implemented.
  void operator=(const vtkDCELL&);  // Not implemented.
};

#endif
