/*=========================================================================

 Program:   Visualization Toolkit
 Module:    $RCSfile: vtkTemporalDataSetSource.cxx,v $

 Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 All rights reserved.
 See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notice for more information.
     
  Modified for better polydata support by Soeren Gebbert
  soerengebbert (at) googlemail (dot) com
     
 =========================================================================*/
#include "vtkTemporalDataSetSource.h"

#include <vtkBitArray.h>
#include <vtkIntArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCell.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkPointData.h>
#include <vtkPointLocator.h>
#include <vtkTemporalDataSet.h>
#include <math.h>

vtkCxxRevisionMacro(vtkTemporalDataSetSource, "$Revision: 1.58 $")
;
vtkStandardNewMacro(vtkTemporalDataSetSource)
;

//-----------------------------------------------------------------------
vtkTemporalDataSetSource::vtkTemporalDataSetSource()
{
  this->TimeRange[0] = 0.0;
  this->TimeRange[1] = 0.0;
  
  this->DiscreteTimeSteps = 0;
  
  this->TimeSteps = vtkDoubleArray::New();
}

//-----------------------------------------------------------------------
vtkTemporalDataSetSource::~vtkTemporalDataSetSource()
{
  this->TimeSteps->Delete();
}

//-----------------------------------------------------------------------
bool vtkTemporalDataSetSource::SetTimeRange(double min, double max, vtkDoubleArray *timeSteps)
{
  if(min < 0.0 || min > max)
  {
    vtkErrorMacro(<< "Range of time steps has wrong values (min is negative or min > max)");
    return false;
  }
  
  this->TimeRange[0] = min;
  this->TimeRange[1] = max;

  double *range = timeSteps->GetRange();
  
  if(timeSteps->GetNumberOfTuples() == 0)
  {
    vtkErrorMacro(<< "Time steps are empty");
    return false;
  }
  
  if(range[0] < min || range[1] > max)
  {
    vtkErrorMacro(<< "Time steps are out of time range");
    return false;
  }
  
  this->SetDiscreteTimeSteps(timeSteps->GetNumberOfTuples());
  
  this->SetNumberOfInputPorts(this->DiscreteTimeSteps);
  
  this->TimeSteps->DeepCopy(timeSteps);
  
  cout << "Number of time steps " << this->DiscreteTimeSteps << endl;
  
  return true;
}

//-----------------------------------------------------------------------
int vtkTemporalDataSetSource::RequestInformation(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  vtkInformation *info=outputVector->GetInformationObject(0);

  // Set the time steps
  if (this->DiscreteTimeSteps)
    {
    double *tsteps = new double[this->DiscreteTimeSteps];
    int i;
    for (i = 0; i < this->DiscreteTimeSteps; ++i)
      {
      tsteps[i] = this->TimeSteps->GetValue(i);
      }
    info->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(),tsteps, this->DiscreteTimeSteps);
    }
  // Set the time range
  info->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(),this->TimeRange,2);

  return 1;
}

//-----------------------------------------------------------------------
int vtkTemporalDataSetSource::RequestData(vtkInformation* vtkNotUsed( request ), vtkInformationVector** inputVector,
                                          vtkInformationVector* outputVector)
{
  int inputPort = 0;
  
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkTemporalDataSet *output = vtkTemporalDataSet::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT())); 
  
  output->SetNumberOfTimeSteps(this->DiscreteTimeSteps);
  
  // The input port is the id of the time step
  for(inputPort = 0; inputPort < this->DiscreteTimeSteps; inputPort++)
  {
    vtkDataSet *input = vtkDataSet::SafeDownCast(inputVector[inputPort]->GetInformationObject(0)->Get(vtkDataObject::DATA_OBJECT()));
    // Attach the input to the temporal data set
    output->SetTimeStep(inputPort, input);
  }
  
  return 1;
}

//-----------------------------------------------------------------------
int vtkTemporalDataSetSource::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
  return 1;
}

//-----------------------------------------------------------------------
void vtkTemporalDataSetSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Number of diskrete time steps: " << this->DiscreteTimeSteps << endl;
  os << indent << "Time range min: " << this->TimeRange[0] << " max: " << this->TimeRange[1] << endl;
}
