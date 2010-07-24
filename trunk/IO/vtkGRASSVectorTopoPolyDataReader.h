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
 * \brief This class reads a grass vector map with topology support from a valid location/mapset
 * and creates a vtkPolyData as output.
 *
 * Areas and isles are supported. Areas are representated as VTK_POLYGON and need to be triangulated
 * for pretty rendering. Additionally the categories, the type of an area (isle or not), the number of isles per
 * area and the size of an area are stored in cell data arrays.
 *
 * The can choose which feature typ should be created as output. Currently centroids, lines, points,
 * boundaries, areas and faces are supported. Only one feature type can be written to the output.
 * If you want to store different features in one poly data set, you need to create one reader for each feature type
 * and merge the output with the vtkAppendFilter.The points may be redundent.
 * Use a point merger to remove redundant points from the dataset.
 *
 * To use this class make sure you are in a grass session and vtkGRASSInit was called first.
 *
 *
 * This class uses the vtkGRASSVecctorMapTopoReader to read the vector map.
 *
 * \TODO Implement Database support to write the dat from a database as cell data arrays.
 * \TODO Use vtkSmartPointer
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

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
  //! \brief Return the chosen feature type. Default is points
  vtkGetMacro(FeatureType, int);

  //!\brief The output will only contain vertices
  virtual void SetFeatureTypeToPoints(){this->SetFeatureType(GV_POINT);}
  //!\brief The output will only contain vertices
  virtual void SetFeatureTypeToCentroid(){this->SetFeatureType(GV_CENTROID);}
  //!\brief The output will only contain polylines
  virtual void SetFeatureTypeToLines(){this->SetFeatureType(GV_LINE);}
  //!\brief The output will only contain polylines
  virtual void SetFeatureTypeToBoundary(){this->SetFeatureType(GV_BOUNDARY);}
  //!\brief The output will only contain ploygons
  virtual void SetFeatureTypeToArea(){this->SetFeatureType(GV_AREA);}
  //!\brief The output will only contain polygons
  virtual void SetFeatureTypeToFace(){this->SetFeatureType(GV_FACE);}

protected:
  vtkGRASSVectorTopoPolyDataReader();
  ~vtkGRASSVectorTopoPolyDataReader();

  vtkSetMacro(FeatureType, int);

  char* VectorName;
  char* Mapset;
  char *CategoryArrayName;

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
