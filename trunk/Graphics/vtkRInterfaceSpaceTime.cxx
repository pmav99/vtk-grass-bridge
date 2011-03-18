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

#include "vtkRInterfaceSpaceTime.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"
#include <vtkStdString.h>
#include <vtkPointSet.h>
#include <vtkPointData.h>
#include <sstream>
#include <vtkFieldData.h>
#include <vtkImageData.h>
#include <vtkStringArray.h>
#include <vtkDataSetAttributes.h>
#include <vtkTemporalDataSet.h>

vtkCxxRevisionMacro(vtkRInterfaceSpaceTime, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkRInterfaceSpaceTime);

//----------------------------------------------------------------------------

vtkRInterfaceSpaceTime::vtkRInterfaceSpaceTime()
{
  // The sp package is needed
  this->EvalRscript("require(\"spacetime\")", true);
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpaceTime::AssignVTKTemporalDataSetToRSpatialTemporalFGDataFrame(vtkTemporalDataSet *temporalData, const char *RVariable, vtkDataArray *timesteps, const char *startDate)
{
  unsigned int timestep;
  bool topoCreated = false;
  unsigned int imageData = 0;
  unsigned int polyData = 0;
  unsigned int numberOfPoints = 0, numberOfCells = 0;
  int numberOfArrays = 0;
  std::ostringstream dataFrameNames;
  std::ostringstream dataFrameName;
  std::ostringstream script;

  // Create the spatial object and attach point data for each time step 
  for (timestep = 0; timestep < temporalData->GetNumberOfTimeSteps(); timestep++) {
    vtkDataSet *data = vtkDataSet::SafeDownCast(temporalData->GetTimeStep(timestep));

    if (timestep == 0) {
      numberOfPoints = data->GetNumberOfPoints();
      numberOfCells = data->GetNumberOfCells();
      numberOfArrays = data->GetPointData()->GetNumberOfArrays();
    } else {
      if (numberOfPoints != data->GetNumberOfPoints() ||
        numberOfCells != data->GetNumberOfCells() ||
        numberOfArrays != data->GetPointData()->GetNumberOfArrays()) {
        vtkErrorMacro( << "Data sets in temporal data set have different number of arrays, points or cells at timestep " << timestep);
        return false;
      }
    }

    // Check for specific supported VTK data sets
    if (data->IsA("vtkImageData") != 1 && data->IsA("vtkPointSet") != 1 && data->IsA("vtkPolyData")) {
      vtkWarningMacro( << "Time step " << timestep << " has an unsupported data set type, skipping");
      continue;
    }

    if (data->IsA("vtkImageData")) {
      imageData++;
      if (topoCreated == false) {
        vtkImageData *imageData = vtkImageData::SafeDownCast(data);
        this->AssignVTKImageDataToRSpatialGrid(imageData, "DataSet");
        topoCreated = true;
      }
    }

    if (data->IsA("vtkPolyData")) {
      polyData++;
      if (topoCreated == false) {
        vtkPointSet *polyData = vtkPointSet::SafeDownCast(data);
        this->AssignVTKPointSetToRSpatialPoints(polyData, "DataSet");
        topoCreated = true;
      }
    }

    if (imageData > 0 && polyData > 0) {
      vtkErrorMacro( << "Mixed data set types are unsupported");
      return false;
    }

    vtkDataSetAttributes *pointData = data->GetPointData();

    dataFrameName.str("");
    dataFrameName << "dataFrame_" << timestep;

    // Attach a single data frame with data for a single time step
    this->AssignVTKDataSetAttributesToRDataFrame(pointData, dataFrameName.str().c_str());

    if (timestep == 0)
      dataFrameNames << dataFrameName.str();
    else
      dataFrameNames << "," << dataFrameName.str();
  }
  
  // Create the time steps
  this->AssignVTKDataArrayToRVariable(timesteps, "timeSteps");
  
  // Create the R time for spacetime
  script << "time = xts(1:" << timesteps->GetNumberOfTuples() << ", as.POSIXct(\"" 
         << startDate << "\") + "<< "timeSteps)";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);
  
  // Append the data frames
  script.str("");
  script << "bind_data = rbind(" << dataFrameNames.str().c_str() << ")";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);
  
  // Create the space time full data frame
  script.str(""); // Clear the script
  script << RVariable << " = STFDF(DataSet, time, bind_data)";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);
  
  // Remove unneeded data
  script.str(""); // Clear the script
  script << "remove(DataSet, time, timeSteps, bind_data, " 
         << dataFrameNames.str().c_str() << ")";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);
  
  return true;
}