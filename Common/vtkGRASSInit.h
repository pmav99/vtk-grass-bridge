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
 * \brief This class should be instanciated in the program at first
 *        It calls the G_gisinit() method of grass, to initialize the grass environment
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 *
 * \TODO Set the grass warning and error function to something which can be handled in C++/Python and Java
 *
 * */

#ifndef __vtkGRASSInit_h
#define __vtkGRASSInit_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

class vtkStringArray;

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSInit : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSInit *New();
  vtkTypeRevisionMacro(vtkGRASSInit,vtkObjectGRASSErrorHandler);

protected:
  vtkGRASSInit();
  ~vtkGRASSInit() {};


private:
  vtkGRASSInit(const vtkGRASSInit&);  // Not implemented.
  void operator=(const vtkGRASSInit&);  // Not implemented.
};


#endif
