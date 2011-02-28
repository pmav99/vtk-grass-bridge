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

#include "vtkRInterfaceSpatial.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"
#include "vtkGRASSVectorPolyDataWriter.h"
#include "vtkGRASSVectorPolyDataReader.h"
#include <vtkStdString.h>
#include <vtkPointSet.h>
#include <vtkPointData.h>
#include <vtkCellDataToPointData.h>
#include <sstream>
#include <vtkFieldData.h>
#include <vtkImageData.h>

vtkCxxRevisionMacro(vtkRInterfaceSpatial, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkRInterfaceSpatial);

//----------------------------------------------------------------------------

vtkRInterfaceSpatial::vtkRInterfaceSpatial()
{
  // The sp package is needed
  this->EvalRscript("require(\"sp\")", true);
  this->Proj4String = NULL;
}

//----------------------------------------------------------------------------
// We convert points and assigned point data into a R 
// SpatialPointsDataFrame from the sp package
// Use the vtlCellDataToPointData to convert cell data into point data
//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKPointSetToRSpatialPointsDataFrame(vtkPointSet *data, const char *name)
{
  std::ostringstream arrayNames;
  std::ostringstream script;
  
  int i;
  
  // Attach the point coordiantes
  this->AssignVTKDataArrayToRVariable(data->GetPoints()->GetData(), data->GetPoints()->GetData()->GetName());
  
  // Assign the point data arrays as R variables
  for(i = 0; i < data->GetPointData()->GetNumberOfArrays(); i++)
  {
    vtkDataArray *array = data->GetPointData()->GetArray(i);
    
    if(i == 0)
      arrayNames << array->GetName();
    else
      arrayNames << "," << array->GetName();
    
    this->AssignVTKDataArrayToRVariable(array, array->GetName());
  }
  
  cout << arrayNames.str() << endl;
  
  // Clear the script for new input
  script.str("");
  
  // Create a dataframe if point data arrays are available
  if(data->GetPointData()->GetNumberOfArrays() > 0)
  {
    // We need to create a data frame from all the attached arrays
    script << "dataframe = data.frame(" << arrayNames.str().c_str() << ")";
    cout << script.str() << endl;
    this->EvalRscript(script.str().c_str(), true);
 
    // Clear the script for new input
    script.str("");

    // Create the SpatialPointsDataFrame
    script << name << " = SpatialPointsDataFrame(" << data->GetPoints()->GetData()->GetName() 
           << ", dataframe";
  } else {
    
    // Clear the script for new input
    script.str("");

    // Create the SpatialPoints
    script << name << " = SpatialPoints(" << data->GetPoints()->GetData()->GetName();
  }
  // Set the projection string if available
  if(this->Proj4String) 
  {
    script << ", proj4string = \"" << this->Proj4String << "\"";
  }
  script << ")";
  
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);
  
  return true;
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKImageDataToRSpatialGridDataFrame(vtkImageData *data, const char *name)
{
  std::ostringstream arrayNames;
  std::ostringstream script;
  
  int i;
  int dim[3]; 
  double origin[3];
  double spacing[3];

  // Get the size of the image
  data->GetDimensions(dim);
  data->GetOrigin(origin);
  data->GetSpacing(spacing);

  // Clear the script for new input
  script.str("");

  // Create the GridTopology
  script << "grid = GridTopology(cellcentre.offset = c(" << origin[0] << "," << origin[1] << "," << origin[2] << ")"; 
  script << ", cellsize = c(" << spacing[0] << "," << spacing[1] << "," << spacing[2] << ")"; 
  script << ", cells.dim = c(" << dim[0] << "," << dim[1] << "," << dim[2] << "))";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);
      
  // Assign the point data arrays as R variables
  for(i = 0; i < data->GetPointData()->GetNumberOfArrays(); i++)
  {
    vtkDataArray *array = data->GetPointData()->GetArray(i);
    
    if(i == 0)
      arrayNames << array->GetName();
    else
      arrayNames << "," << array->GetName();
    
    this->AssignVTKDataArrayToRVariable(array, array->GetName());
  }
  
  cout << arrayNames.str() << endl;
  
  // Clear the script for new input
  script.str("");
  
  // Create a dataframe if point data arrays are available
  if(data->GetPointData()->GetNumberOfArrays() > 0)
  {
    // We need to create a data frame from all the attached arrays
    script << "dataframe = data.frame(" << arrayNames.str().c_str() << ")";
    cout << script.str() << endl;
    this->EvalRscript(script.str().c_str(), true);
 
    // Clear the script for new input
    script.str("");

    // Create the SpatialGridDataFrame
    script << name << " = SpatialGridDataFrame(grid"  << ", dataframe";
  } else {
    
    // Clear the script for new input
    script.str("");

    // Create the SpatialPoints
    script << name << " = SpatialGrid(grid";
  }
  // Set the projection string if available
  if(this->Proj4String) 
  {
    script << ", proj4string = \"" << this->Proj4String << "\"";
  }
  script << ")";
  
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);
  
  return true;
}