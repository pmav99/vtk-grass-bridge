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
 * \brief This class reads the basic grass database informations
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSDatabaseInfo_h
#define __vtkGRASSDatabaseInfo_h

#include "vtkGRASSBridgeCommonWin32Header.h"
#include "vtkObjectGRASSErrorHandler.h"

class vtkStringArray;

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSDatabaseInfo : public vtkObjectGRASSErrorHandler
{
public:
  static vtkGRASSDatabaseInfo *New();
  vtkTypeRevisionMacro(vtkGRASSDatabaseInfo,vtkObjectGRASSErrorHandler);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  vtkGetStringMacro(CurrentLocationName);
  vtkGetStringMacro(CurrentMapsetName);
  vtkGetStringMacro(CurrentLocationTitle);
  vtkGetStringMacro(GisBasePath);
  vtkGetStringMacro(CurrentLocationPath);
  vtkGetMacro(Projection, int);

  vtkGetObjectMacro(AvailableMapSets, vtkStringArray);

  virtual bool Refresh();

protected:
  vtkGRASSDatabaseInfo();
  ~vtkGRASSDatabaseInfo();

  vtkSetStringMacro(CurrentLocationName);
  vtkSetStringMacro(CurrentMapsetName);
  vtkSetStringMacro(CurrentLocationTitle);
  vtkSetStringMacro(GisBasePath);
  vtkSetStringMacro(CurrentLocationPath);
  vtkSetMacro(Projection, int);

  char *CurrentLocationName;
  char *CurrentMapsetName;
  char *CurrentLocationTitle;
  char *GisBasePath;
  char *CurrentLocationPath;
  int Projection;
  vtkStringArray *AvailableMapSets;

private:
  vtkGRASSDatabaseInfo(const vtkGRASSDatabaseInfo&);  // Not implemented.
  void operator=(const vtkGRASSDatabaseInfo&);  // Not implemented.
};


#endif
