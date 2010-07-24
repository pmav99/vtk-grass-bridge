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
 * read a raster map row by row.
 *
 * The user can choose the current region, the default region, the raster map region or
 * a user defined region to open the raster map.
 *
 * This class does not use the vtk update mechanism.
 * Before reading the raster map rows you need to open the raster map.
 * After reading you must close the raster map or delete the object. The
 * destructor will close the map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRasterMapReader_h
#define __vtkGRASSRasterMapReader_h

#include <vtkGRASSRasterMapBase.h>
#include "vtkGRASSBridgeRasterWin32Header.h"

class VTK_GRASS_BRIDGE_RASTER_EXPORT vtkGRASSRasterMapReader : public vtkGRASSRasterMapBase
{
public:
  static  vtkGRASSRasterMapReader *New();
  vtkTypeRevisionMacro(vtkGRASSRasterMapReader,vtkGRASSRasterMapBase);

  //! \brief Open an existing raster map for reading.
  //! \param name the name of an existing raster map
  //! \return true for success, false fo error
  virtual bool OpenMap(char *name);
  
  //! \brief Return the min and max range as double for all map types.
  //! Call only after the raster map was opened.
  //! \return true for success, false for error
  virtual bool GetRange(double range[2]);
protected:
  vtkGRASSRasterMapReader(){};
  ~vtkGRASSRasterMapReader(){};

private:

    vtkGRASSRasterMapReader(const vtkGRASSRasterMapReader&);  // Not implemented.
  void operator=(const vtkGRASSRasterMapReader&);  // Not implemented.
};


#endif
