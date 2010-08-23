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

#ifndef __vtkGRASSRaster3dMapReader_h
#define __vtkGRASSRaster3dMapReader_h

#include <vtkGRASSRaster3dMapBase.h>
#include "vtkGRASSBridgeRaster3dWin32Header.h"

class VTK_GRASS_BRIDGE_RASTER3D_EXPORT vtkGRASSRaster3dMapReader : public vtkGRASSRaster3dMapBase
{
public:
  static  vtkGRASSRaster3dMapReader *New();
  vtkTypeRevisionMacro(vtkGRASSRaster3dMapReader,vtkGRASSRaster3dMapBase);

  //! \brief Open an existing 3d raster map for reading.
  //! \param name the name of an existing 3d raster map
  //! \return true for success, false fo error
  virtual bool OpenMap(char *name);
  
  //! \brief Return the min and max range as double for all map types.
  //! Call only after the 3d raster map was opened.
  //! \return true for success, false for error
  virtual bool GetRange(double range[2]);
protected:
  vtkGRASSRaster3dMapReader(){};
  ~vtkGRASSRaster3dMapReader(){};

private:

    vtkGRASSRaster3dMapReader(const vtkGRASSRaster3dMapReader&);  // Not implemented.
  void operator=(const vtkGRASSRaster3dMapReader&);  // Not implemented.
};


#endif
