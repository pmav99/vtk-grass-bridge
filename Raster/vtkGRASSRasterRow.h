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
 * \brief This class represents a single raster row of GRASS GIS raster maps
 *
 * Methods for allocation, deallocation and value access are provided.
 * 
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRasterRow_h
#define __vtkGRASSRasterRow_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster.h>
}

class vtkDCELL;
class vtkFCELL;
class vtkCELL;

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSRasterRow : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSRasterRow *New();
  vtkTypeRevisionMacro(vtkGRASSRasterRow, vtkObject);

  //!\brief Allocate a raster row of type DCELL, removes previously allocated data
  //!\param cols The number of cols to allocate
  bool AllocateDCELL(int cols);
  //!\brief Allocate a raster row of type FCELL, removes previously allocated data
  //!\param cols The number of cols to allocate
  bool AllocateFCELL(int cols);
  //!\brief Allocate a raster row of type CELL, removes previously allocated data
  //!\param cols The number of cols to allocate
  bool AllocateCELL(int cols);
  //!\brief Check if the row was allocated
  bool IsAllocated();

  //!\brief Get the DCELL value of the row at position col
  //! The raster row must be of type DCELL, otherwise false is returned
  bool GetDCELLValue(int col, vtkDCELL *value);
  //!\brief Get the FCELL value of the row at position col
  //! The raster row must be of type FCELL, otherwise false is returned
  bool GetFCELLValue(int col, vtkFCELL *value);
  //!\brief Get the CELL value of the row at position col
  //! The raster row must be of type CELL, otherwise false is returned
  bool GetCELLValue(int col, vtkCELL *value);

  //!\brief Set the DCELL value of the row at position col
  //! The raster row must be of type DCELL, otherwise false is returned
  bool SetDCELLValue(int col, vtkDCELL *value);
  //!\brief Set the FCELL value of the row at position col
  //! The raster row must be of type DCELL, otherwise false is returned
  bool SetFCELLValue(int col, vtkFCELL *value);
  //!\brief Set the CELL value of the row at position col
  //! The raster row must be of type DCELL, otherwise false is returned
  bool SetCELLValue(int col, vtkCELL *value);

  //!\brief Check if column is of type Null
  bool IsNull(int col);
  //!\brief Set the row at position col to Null
  bool SetToNull(int col);
  //!\brief Set the raster row to Null
  bool SetToNull();

  //!\brief Reset the raster row and free internal memoryused for row storage
  void Reset();

  //!\brief convert the raster row into a vtkDataArray. The type of the
  //! data array depends on the row type:
  //! CELL  -> vtkIntArray
  //! FCELL -> vtkFloatArray
  //! DCELL -> vtkDoubleArray
  //!\return NULL in case of failure
  vtkDataArray *ToDataArray();
  
  vtkGetMacro(NumberOfColumns, int);
  vtkGetMacro(RowType, int);

protected:
  vtkGRASSRasterRow(){;}
  ~vtkGRASSRasterRow() {};

  bool Allocated;
  int RowType;
  int NumberOfColumns;
  CELL *CELLBuff;
  FCELL *FCELLBuff;
  DCELL *DCELLBuff;

private:
  vtkGRASSRasterRow(const vtkGRASSRasterRow&);  // Not implemented.
  void operator=(const vtkGRASSRasterRow&);  // Not implemented.
};

#endif
