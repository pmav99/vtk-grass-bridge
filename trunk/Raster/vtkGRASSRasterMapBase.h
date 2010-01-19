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

#ifndef __vtkGRASSRasterMapBase_h
#define __vtkGRASSRasterMapBase_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeRasterWin32Header.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSHistory.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster.h>
}

class vtkCharArray;


class VTK_GRASS_BRIDGE_RASTER_EXPORT vtkGRASSRasterMapBase : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSRasterMapBase *New();
  vtkTypeRevisionMacro(vtkGRASSRasterMapBase,vtkObjectGRASSErrorHandler);

  vtkGetStringMacro(RasterName);
  vtkGetStringMacro(Mapset);

  virtual void UseDefaultRegion(){this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);}
  virtual void UseCurrentRegion(){this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);}
  virtual void UseRasterRegion(){this->SetRegionUsage(VTK_GRASS_REGION_RASTER);}
  virtual void UseUserDefinedRegion(){this->SetRegionUsage(VTK_GRASS_REGION_USER);}
  vtkGetMacro(RegionUsage, int);

  //! \brief Set the Region which should be used to open the grass raster map
  vtkSetObjectMacro(Region, vtkGRASSRegion);
  vtkGetObjectMacro(Region, vtkGRASSRegion);

    //! \brief Get the History of the grass raster map
  vtkGetObjectMacro(History, vtkGRASSHistory);

  //! \brief open the map for reading or writing. This function must be implemented by the subclasses.
  virtual bool OpenMap(char *name){return false;}

  //! \brief close the map. Must be implemented in the subclass
  virtual bool CloseMap(){return false;}

  //! \brief Read a row of the map and return the content as vtkDataArray
  virtual vtkDataArray *GetRow(int idx);
  virtual vtkCharArray *GetNullRow(int idx);

  vtkGetMacro(MapType, int);
  vtkGetMacro(NumberOfRows, int);
  vtkGetMacro(NumberOfCols, int);

  //! \brief Returns true if the map is open
  //! \return true for open, false for not open
  virtual bool IsOpen() {return this->Open;}

  //! \brief Null value which should replace the default grass null value for CELL, FCELL andDCELL maps
  //! to enable the NullValue, set the this->UseGRASSNulleValueOff()
  vtkSetMacro(NullValue, double);
  //! \brief Null value which should replace the default grass null value for CELL, FCELL andDCELL maps
  //! to enable the NullValue, set the this->UseGRASSNulleValueOff() 
  vtkGetMacro(NullValue, double);


  //! \brief Use the grass NULL value to be stored in the vtkDataArray
  vtkGetMacro(UseGRASSNulleValue, int);
  //! \brief Use the grass NULL value to be stored in the vtkDataArray
  vtkBooleanMacro(UseGRASSNulleValue, int);
  

protected:
  vtkGRASSRasterMapBase();
  ~vtkGRASSRasterMapBase();


  vtkSetMacro(UseGRASSNulleValue, int);

  int RegionUsage;
  bool Open; // True if the raster file was opened
  int Map;     // File descriptor
  int MapType; //CELL; FCELL or DCELL
  int NumberOfRows;
  int NumberOfCols;
  char *RasterName;
  char *Mapset;
  double NullValue;
  int UseGRASSNulleValue;

  vtkGRASSRegion *Region;
  vtkGRASSHistory *History;
  //BTX
//  struct Range crange;
//  struct FPRange range;
//  struct Categories cats;
//  struct History hist;
//  struct TimeStamp ts;
  vtkSmartPointer<vtkDataArray> Row;
  vtkSmartPointer<vtkCharArray> NullRow;
  double *RasterBuff;
  char *NullBuff;
  //ETX
  // Colortable, history and cats, timestamp are needed

  //! \brief Set the region
  bool SetRegion();
  //! \brief Allocate the grass raster buffer and the vtkDädataArray. 
  //! The SetRegion() must be called befor this method. And the Map must be opened.
  bool SetUpRasterBuffer();

  //! \brief Name of the raster map to be readed or created
  vtkSetStringMacro(RasterName);
  vtkSetStringMacro(Mapset);
  vtkSetMacro(MapType, int);
  vtkSetMacro(RegionUsage, int);

private:
  vtkGRASSRasterMapBase(const vtkGRASSRasterMapBase&);  // Not implemented.
  void operator=(const vtkGRASSRasterMapBase&);  // Not implemented.
};


#endif
