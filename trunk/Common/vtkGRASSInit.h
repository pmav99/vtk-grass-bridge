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
 * An error function can be set to catch the grass G_fatal_error calles and avoid
 * exit of the program.
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
  
  //! \brief Exit if an error within grass occures -> in case G_fatal_error() was called.
  void ExitOnErrorOn();
  //! \brief Do not exit on error -> in case G_fatal_error() was called.
  void ExitOnErrorOff();
  //!\brief Init GRASS
  void Init(const char *name);
  //!\brief Parse the command line arguments each argument is a single string in the string array
  void Parser(vtkStringArray *argv);
  //BTX
  //!\brief Prase the command line arguments
  void Parser(int argc, char **argv);
  //ETX
protected:
  vtkGRASSInit();
  ~vtkGRASSInit() {};


private:
  vtkGRASSInit(const vtkGRASSInit&);  // Not implemented.
  void operator=(const vtkGRASSInit&);  // Not implemented.
};


#endif
