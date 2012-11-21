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
 * \brief This class writes PolyData build from vertical lines as
 * GRASS raster maps.
 *
 * Raster maps are written for each vertical layer and each cell data array
 * associated with the PolyData.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2012
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSMultiRasterPolyDataLineWriter_h
#define __vtkGRASSMultiRasterPolyDataLineWriter_h

#ifndef NULL
#define NULL 0
#endif

#include <vtkStringArray.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataAlgorithm.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSRasterMapWriter.h"
#include "vtkGRASSRegion.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSMultiRasterPolyDataLineWriter : public vtkPolyDataAlgorithm
{
public:
  static vtkGRASSMultiRasterPolyDataLineWriter* New();
  vtkTypeRevisionMacro(vtkGRASSMultiRasterPolyDataLineWriter,vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //!\brief Get the array name that replace the raster names in the cell data
  vtkGetStringMacro(RasterBaseName);
  //!\brief Set the array name that replace the raster names in the cell data
  vtkSetStringMacro(RasterBaseName);

  vtkGetObjectMacro(RasterMapNames, vtkStringArray);

protected:
  vtkGRASSMultiRasterPolyDataLineWriter();
  ~vtkGRASSMultiRasterPolyDataLineWriter();

  char *RasterBaseName;
  vtkStringArray *RasterMapNames;

  virtual int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);

private:
  vtkGRASSMultiRasterPolyDataLineWriter(const vtkGRASSMultiRasterPolyDataLineWriter&);  // Not implemented.
  void operator=(const vtkGRASSMultiRasterPolyDataLineWriter&);  // Not implemented.
};

#endif
