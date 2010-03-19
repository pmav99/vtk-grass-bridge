
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
 * \brief This class provides an interface to the raster map io library to
 * write a raster map row by row.
 *
 * The user can choose the current region, the default region, the raster map region or
 * a user defined region to open the new raster map.
 *
 * This class does not use the vtk update mechanism.
 * Before writing the new raster map rows you need to open the raster map.
 * After writing you must close the raster map or delete the object. The
 * destructor will close the map.
 * 
 * The user can provide a vtkGRASSHistory object, which will be used to write the history
 * of the map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */
#ifndef __vtkGRASSRasterMapWriter_h
#define __vtkGRASSRasterMapWriter_h

#include <vtkGRASSRasterMapBase.h>
#include "vtkGRASSBridgeRasterWin32Header.h"

class vtkDataArray;

class VTK_GRASS_BRIDGE_RASTER_EXPORT vtkGRASSRasterMapWriter : public vtkGRASSRasterMapBase
{
public:
  static  vtkGRASSRasterMapWriter *New();
  vtkTypeRevisionMacro(vtkGRASSRasterMapWriter,vtkGRASSRasterMapBase);

  virtual void SetMapTypeToCELL() {this->SetMapType(CELL_TYPE);}
  virtual void SetMapTypeToFCELL() {this->SetMapType(FCELL_TYPE);}
  virtual void SetMapTypeToDCELL() {this->SetMapType(DCELL_TYPE);}

  //! \brief Set the histroy which should be written to the raster map
  vtkSetObjectMacro(History, vtkGRASSHistory);

  //! \brief Open old raster map for reading
  virtual bool OpenMap(char *name);
  //! \brief Write a rast row, returns 1 on success, -1 in case of an error
  virtual int PutNextRow(vtkDataArray *data);
  //! \brief close Map and write a short history
  virtual bool CloseMap();
  //! \brief the current position of the row in the new raster map
  vtkSetStringMacro(MapTitle);
  vtkGetStringMacro(MapTitle);
  
protected:
  vtkGRASSRasterMapWriter();
  ~vtkGRASSRasterMapWriter(){};

private:

    char* MapTitle;

    vtkGRASSRasterMapWriter(const vtkGRASSRasterMapWriter&);  // Not implemented.
  void operator=(const vtkGRASSRasterMapWriter&);  // Not implemented.
};


#endif