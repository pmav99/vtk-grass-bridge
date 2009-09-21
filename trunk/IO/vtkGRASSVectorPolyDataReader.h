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

#ifndef __vtkGRASSVectorPolyDataReader_h
#define __vtkGRASSVectorPolyDataReader_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkGRASSBridgeIOWin32Header.h"

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSVectorPolyDataReader : public vtkPolyDataAlgorithm
{
public:
  static vtkGRASSVectorPolyDataReader* New();
  vtkTypeRevisionMacro(vtkGRASSVectorPolyDataReader,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetStringMacro(VectorName);
  vtkGetStringMacro(VectorName);
  vtkGetStringMacro(Mapset);

protected:
  vtkGRASSVectorPolyDataReader();
  ~vtkGRASSVectorPolyDataReader();

  char* VectorName;
  char* Mapset;

  vtkSetStringMacro(Mapset);

  int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);
private:
  vtkGRASSVectorPolyDataReader(const vtkGRASSVectorPolyDataReader&);  // Not implemented.
  void operator=(const vtkGRASSVectorPolyDataReader&);  // Not implemented.
};

#endif
