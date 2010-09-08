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
 * \brief This class should be used to create vtkGRASSOption objects of a
 * specific type (raster input, vector output, ...).
 *
 * Several
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 *
 * \TODO
 *
 * */

#ifndef __vtkGRASSOptionFactory_h
#define __vtkGRASSOptionFactory_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSOptionFactory : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSOptionFactory *New();
  vtkTypeRevisionMacro(vtkGRASSOptionFactory,vtkObjectGRASSErrorHandler);

  protected:
  vtkGRASSOptionFactory();
  ~vtkGRASSOptionFactory() {};

private:
  vtkGRASSOptionFactory(const vtkGRASSOptionFactory&);  // Not implemented.
  void operator=(const vtkGRASSOptionFactory&);  // Not implemented.
};

#endif

