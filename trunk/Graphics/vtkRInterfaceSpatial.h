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
 * */

#ifndef __vtkRInterfaceSpatial_h
#define __vtkRInterfaceSpatial_h

#include <vtkRInterface.h>
#include "vtkGRASSBridgeGraphicsWin32Header.h"

class vtkPointSet;
class vtkImageData;

class VTK_GRASS_BRIDGE_GRAPHICS_EXPORT vtkRInterfaceSpatial : public vtkRInterface
{
public:
  static  vtkRInterfaceSpatial *New();
  vtkTypeRevisionMacro(vtkRInterfaceSpatial, vtkRInterface);
  
  virtual bool AssignVTKPointSetToRSpatialPointsDataFrame(vtkPointSet *data, const char *name);
  virtual bool AssignVTKImageDataToRSpatialGridDataFrame(vtkImageData *data, const char *name);
  
  //!\brief Set the string which defines the coordiante reference system in proj4 format
  vtkSetStringMacro(Proj4String);
  //!\brief The string which defines the coordiante reference system in proj4 format
  vtkGetStringMacro(Proj4String);

  //ETX
protected:
  vtkRInterfaceSpatial();
  ~vtkRInterfaceSpatial() {};
  
  char *Proj4String;

private:
  vtkRInterfaceSpatial(const vtkRInterfaceSpatial&);  // Not implemented.
  void operator=(const vtkRInterfaceSpatial&);  // Not implemented.
};


#endif
