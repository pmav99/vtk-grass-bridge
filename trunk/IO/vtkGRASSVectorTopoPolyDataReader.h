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

#ifndef __vtkGRASSVectorTopoPolyDataReader_h
#define __vtkGRASSVectorTopoPolyDataReader_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkGRASSBridgeIOWin32Header.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSVectorTopoPolyDataReader : public vtkPolyDataAlgorithm
{
public:
  static vtkGRASSVectorTopoPolyDataReader* New();
  vtkTypeRevisionMacro(vtkGRASSVectorTopoPolyDataReader,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetStringMacro(VectorName);
  vtkGetStringMacro(VectorName);
  vtkGetStringMacro(Mapset);

  vtkGetMacro(FeatureType, int);

  virtual void SetFeatureTypeToPoints(){this->SetFeatureType(GV_POINTS);}
  virtual void SetFeatureTypeToCentroid(){this->SetFeatureType(GV_CENTROID);}
  virtual void SetFeatureTypeToLines(){this->SetFeatureType(GV_LINES);}
  virtual void SetFeatureTypeToBoundary(){this->SetFeatureType(GV_BOUNDARY);}
  virtual void SetFeatureTypeToArea(){this->SetFeatureType(GV_AREA);}
  virtual void SetFeatureTypeToFace(){this->SetFeatureType(GV_FACE);}

protected:
  vtkGRASSVectorTopoPolyDataReader();
  ~vtkGRASSVectorTopoPolyDataReader();

  vtkSetMacro(FeatureType, int);

  char* VectorName;
  char* Mapset;

  int FeatureType;

  vtkSetStringMacro(Mapset);

  int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);
private:
  vtkGRASSVectorTopoPolyDataReader(const vtkGRASSVectorTopoPolyDataReader&);  // Not implemented.
  void operator=(const vtkGRASSVectorTopoPolyDataReader&);  // Not implemented.
};

#endif
