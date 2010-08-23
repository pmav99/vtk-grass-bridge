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
 * \brief This is the base class for grass raster map reading and writing.
 *
 * It provides function to allocate the raster buffer, to set and get the
 * vtkGRASSRegion and the vtkGRASSHistory.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRaster3dMapBase_h
#define __vtkGRASSRaster3dMapBase_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeRaster3dWin32Header.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSHistory.h"

extern "C"{
#include <grass/gis.h>
#include <grass/G3d.h>
#include <grass/raster.h>
}

class VTK_GRASS_BRIDGE_RASTER3D_EXPORT vtkGRASSRaster3dMapBase : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSRaster3dMapBase *New();
  vtkTypeRevisionMacro(vtkGRASSRaster3dMapBase,vtkObjectGRASSErrorHandler);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkGetStringMacro(Raster3dName);
  vtkGetStringMacro(Mapset);

  virtual void UseDefaultRegion(){this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);}
  virtual void UseCurrentRegion(){this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);}
  virtual void UseRasterRegion(){this->SetRegionUsage(VTK_GRASS_REGION_RASTER);}
  virtual void UseUserDefinedRegion(){this->SetRegionUsage(VTK_GRASS_REGION_USER);}
  vtkGetMacro(RegionUsage, int);

  //!\brief Return the maptype as string (FCELL_TYPE and DCELL_TYPE or NULL if unknown)
  virtual const char* GetMapTypeAsString();

  //! \brief Set the Region which should be used to open the grass raster map
  vtkSetObjectMacro(Region, vtkGRASSRegion);
  vtkGetObjectMacro(Region, vtkGRASSRegion);

    //! \brief Get the History of the grass 3d raster map
  vtkGetObjectMacro(History, vtkGRASSHistory);
  
  //! \brief open the map for reading or writing. This function must be implemented by the subclasses.
  virtual bool OpenMap(char *name){return false;}

  //! \brief Close the raster map
  virtual bool CloseMap();

  //! \brief Read a single value on index position x, y and z
  //! Double is always returned, independent from the map type
  virtual double GetValue(int x, int y, int z);

  vtkGetMacro(MapType, int);
  vtkGetMacro(NumberOfRows, int);
  vtkGetMacro(NumberOfCols, int);
  vtkGetMacro(NumberOfDepths, int);

  //! \brief Returns true if the map is open
  //! \return true for open, false for not open
  virtual bool IsOpen() {return this->Open;}
  

protected:
  vtkGRASSRaster3dMapBase();
  ~vtkGRASSRaster3dMapBase();
  
  int RegionUsage;
  bool Open; // True if the raster file was opened
  int MapType; // FCELL or DCELL
  int NumberOfRows;
  int NumberOfCols;
  int NumberOfDepths;
  char *Raster3dName;
  char *Mapset;

  //BTX
  G3D_Map *Map;  // Pointer to the g3d map
  //ETX

  vtkGRASSRegion *Region;
  vtkGRASSHistory *History;

  //! \brief Set the region
  bool SetRegion();

  //! \brief Name of the raster map to be readed or created
  vtkSetStringMacro(Raster3dName);
  vtkSetStringMacro(Mapset);
  vtkSetMacro(MapType, int);
  vtkSetMacro(RegionUsage, int);

private:
  vtkGRASSRaster3dMapBase(const vtkGRASSRaster3dMapBase&);  // Not implemented.
  void operator=(const vtkGRASSRaster3dMapBase&);  // Not implemented.
};


#endif
