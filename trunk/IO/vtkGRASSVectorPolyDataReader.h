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
 * \brief This class reads a grass vector map without topology support from a valid location/mapset
 * and creates a vtkPolyData as output.
 *
 * Areas and isles are not supported. The only data types which are supported are points and lines.
 * The category is saved as an integer cell data array and set to active. The points may be redundent.
 * Use a point merger to remove redundant points from the dataset.
 *
 * To use this class make sure you are in a grass session and vtkGRASSInit was called first.
 *
 * This class uses the vtkGRASSVecctorMapNoTopoReader to read the vector map.
 *
 * \TODO Implement Database support to write the dat from a database as cell data arrays.
 * \TODO Use vtkSmartPointer
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

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

  //! \brief Set the name of vector map
  vtkSetStringMacro(VectorName);
  //! \brief Set the name of vector map
  vtkGetStringMacro(VectorName);
  //! \brief Set the name of the mapset the vector map was read from
  vtkGetStringMacro(Mapset);
  //! \brief Set the name of the category array
  vtkSetStringMacro(CategoryArrayName);
  //! \brief Get the name of the category array
  vtkGetStringMacro(CategoryArrayName);

protected:
  vtkGRASSVectorPolyDataReader();
  ~vtkGRASSVectorPolyDataReader();

  char* VectorName;
  char* Mapset;
  char *CategoryArrayName;

  vtkSetStringMacro(Mapset);

  int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);
private:
  vtkGRASSVectorPolyDataReader(const vtkGRASSVectorPolyDataReader&);  // Not implemented.
  void operator=(const vtkGRASSVectorPolyDataReader&);  // Not implemented.
};

#endif
