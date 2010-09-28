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

#include "vtkGRASSRasterImageReader.h"

#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSRasterMapReader.h"
#include "vtkDataArray.h"

extern "C"
{
#include <grass/gis.h>
#include <math.h>
}

vtkCxxRevisionMacro(vtkGRASSRasterImageReader, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSRasterImageReader);

//----------------------------------------------------------------------------

vtkGRASSRasterImageReader::vtkGRASSRasterImageReader()
{
    this->DataExtent[0] = 0;
    this->DataExtent[1] = 255;
    this->DataExtent[2] = 0;
    this->DataExtent[3] = 255;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = 0;

    this->DataScalarType = VTK_DOUBLE;
    this->MapTypeConversion = 0; // Use the maptype as scalar type

    this->DataOrigin[0] = this->DataOrigin[1] = this->DataOrigin[2] = 0.0;
    this->DataSpacing[0] = this->DataSpacing[1] = this->DataSpacing[2] = 1.0;

    this->RasterName = NULL;
    this->Mapset = NULL;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);

    this->RegionUsage = VTK_GRASS_REGION_CURRENT;

    this->RasterMap = vtkGRASSRasterMapReader::New();

    this->NullValue = this->RasterMap->GetNullValue();
    this->UseNullValue = 1;

    //G_gisinit("vtkGRASSRasterImageReader");
}

//----------------------------------------------------------------------------

vtkGRASSRasterImageReader::~vtkGRASSRasterImageReader()
{
    if (this->RasterName)
        delete [] this->RasterName;
    if (this->Mapset)
        delete [] this->Mapset;
    this->RasterMap->Delete();
}

//----------------------------------------------------------------------------

int
vtkGRASSRasterImageReader::RequestInformation(
                                              vtkInformation * vtkNotUsed(request),
                                              vtkInformationVector ** vtkNotUsed(inputVector),
                                              vtkInformationVector *outputVector)
{


    if (this->RegionUsage == VTK_GRASS_REGION_CURRENT)
    {
        this->RasterMap->UseCurrentRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT)
    {

        this->RasterMap->UseDefaultRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_RASTER)
    {
        this->RasterMap->UseRasterRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_USER)
    {
        this->RasterMap->UseUserDefinedRegion();
    }

    this->RasterMap->SetNullValue(this->NullValue);
    if(this->UseNullValue)
        this->RasterMap->UseNullValueOn();

    // The region settings are made in the OpenMap method.
    // So we need to open the map here to get the correct
    // region settings.
    if (!this->RasterMap->OpenMap(this->RasterName))
    {
        vtkErrorMacro( << "Error in vtkGRASSRasterMap: " << this->RasterMap->GetError());
        return 0;
    }


    if(this->MapTypeConversion == 0) {
        if (this->RasterMap->GetMapType() == CELL_TYPE)
        {
            this->SetDataScalarTypeToInt();
        }
        else if (this->RasterMap->GetMapType() == FCELL_TYPE)
        {
            this->SetDataScalarTypeToFloat();
        }
        else
        {
            this->SetDataScalarTypeToDouble();
        }
    }

    if(this->MapTypeConversion == 1)
        this->SetDataScalarTypeToInt();
    if(this->MapTypeConversion == 2)
        this->SetDataScalarTypeToFloat();
    if(this->MapTypeConversion == 3)
        this->SetDataScalarTypeToDouble();

    // Init the image extent with the grass region settings
    this->DataExtent[0] = 0;
    this->DataExtent[1] = this->RasterMap->GetNumberOfCols() - 1;
    this->DataExtent[2] = 0;
    this->DataExtent[3] = this->RasterMap->GetNumberOfRows() - 1;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = 0;

    this->DataSpacing[0] = this->RasterMap->GetRegion()->GetEastWestResolution();
    this->DataSpacing[1] = this->RasterMap->GetRegion()->GetNorthSouthResolution();
    // The Scalars are point data. So the points are located in the center of the grass pixel
    // so we have to adjust the origin.
    this->DataOrigin[0] = this->RasterMap->GetRegion()->GetWesternEdge() +
        this->RasterMap->GetRegion()->GetEastWestResolution() / 2.0;
    this->DataOrigin[1] = this->RasterMap->GetRegion()->GetSouthernEdge() +
        this->RasterMap->GetRegion()->GetNorthSouthResolution() / 2.0;
    this->DataOrigin[2] = 0;

    // get the info objects
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    outInfo->Set(vtkDataObject::SPACING(), this->DataSpacing, 3);
    outInfo->Set(vtkDataObject::ORIGIN(), this->DataOrigin, 3);
    outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
                 this->DataExtent, 6);
    vtkDataObject::SetPointDataActiveScalarInfo(outInfo, this->DataScalarType, 1);
    return 1;
}

//----------------------------------------------------------------------------

template<class T>
void
vtkGRASSRasterImageReaderExecute(vtkGRASSRasterImageReader *self,
                                 vtkImageData *data, T *outPtr,
                                 int outExt[6])
{

    int idxX, idxY, idxZ;
    vtkIdType outIncX, outIncY, outIncZ;
    int rowcount = outExt[3];
    vtkGRASSRasterMapReader *map = self->GetRasterMap();
    vtkDataArray *row;

    // Get increments to march through data
    data->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);


    // Loop through output pixel
    for (idxZ = outExt[4]; idxZ <= outExt[5]; idxZ++)
    {
        for (idxY = outExt[2]; !self->GetAbortExecute() && idxY <= outExt[3]; idxY++)
        {

            row = map->GetRow(rowcount);
            rowcount--;

            for (idxX = outExt[0]; idxX <= outExt[1]; idxX++)
            {
                *outPtr++ = (T)row->GetTuple1(idxX);
            }
            outPtr += outIncY;
        }
        outPtr += outIncZ;
    }
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterImageReader::ExecuteData(vtkDataObject *output)
{

    vtkImageData *data = this->AllocateOutputData(output);
    int *outExt = data->GetExtent();
    void *outPtr = data->GetScalarPointerForExtent(outExt);


    // Call the correct templated function for the output
    switch (this->GetDataScalarType()) {
        vtkTemplateMacro(vtkGRASSRasterImageReaderExecute(this, data,
                                                          static_cast<VTK_TT *> (outPtr),
                                                          outExt));
    default:
        vtkErrorMacro( << "Execute: Unknown data type");
    }

    // All work is done. now we can close the map
    this->RasterMap->CloseMap();
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterImageReader::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "DataScalarType: " <<
        vtkImageScalarTypeNameMacro(this->DataScalarType) << "\n";
    os << indent << "DataExtent: (" << this->DataExtent[0] << ", "
        << this->DataExtent[1] << ", "
        << this->DataExtent[2] << ", "
        << this->DataExtent[3] << ", "
        << this->DataExtent[4] << ", "
        << this->DataExtent[5] << ")\n";
    os << indent << "DataSpacing: (" << this->DataSpacing[0] << ", "
        << this->DataSpacing[1] << ", "
        << this->DataSpacing[2] << ")\n";
    os << indent << "DataOrigin: (" << this->DataOrigin[0] << ", "
        << this->DataOrigin[1] << ", "
        << this->DataOrigin[2] << ")\n";
}
