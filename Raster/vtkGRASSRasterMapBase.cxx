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

#include "vtkGRASSRasterMapBase.h"
#include "vtkGRASSRasterToImageReader.h"
#include <vtkStringArray.h>
#include <vtkCharArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSHistory.h"
#include <vtkGRASSDefines.h>

vtkCxxRevisionMacro(vtkGRASSRasterMapBase, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSRasterMapBase);

//----------------------------------------------------------------------------

vtkGRASSRasterMapBase::vtkGRASSRasterMapBase()
{
    this->RasterName = NULL;
    this->Mapset = NULL;
    this->Region = vtkGRASSRegion::New();
    this->History = vtkGRASSHistory::New();
    this->Map = -1;
    this->MapType = CELL_TYPE;
    this->NumberOfRows = 0;
    this->NumberOfCols = 0;
    this->Open = false;
    this->Row = NULL;
    this->NullRow = vtkSmartPointer<vtkCharArray>::New();
    this->RegionUsage = VTK_GRASS_REGION_CURRENT;
    this->RasterBuff = (double*) NULL;
    this->NullBuff = (char*) NULL;
    this->NullValue = -9999;
    this->UseGRASSNulleValue = 1;
}

//----------------------------------------------------------------------------

vtkGRASSRasterMapBase::~vtkGRASSRasterMapBase()
{
    this->CloseMap();

    if (this->RasterName)
        delete [] this->RasterName;
    if (this->Mapset)
        delete [] this->Mapset;

    this->Region->Delete();
    this->History->Delete();

    if (this->RasterBuff)
        G_free(this->RasterBuff);
}


//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapBase::SetRegion()
{
    struct Cell_head head;

    if (this->RasterName == NULL || this->Mapset == NULL)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to set the region. RasterName is not set.",
                   this->GetClassName(), __LINE__);
        return false;
    }
    TRY
        if (this->RegionUsage == VTK_GRASS_REGION_CURRENT)
        {
            G_get_set_window(&head);
            this->Region->CopyRegionFrom(&head);
        }
        else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT)
        {
            G_get_default_window(&head);
            G_set_window(&head);
            this->Region->CopyRegionFrom(&head);
        }
        else if (this->RegionUsage == VTK_GRASS_REGION_RASTER)
        {
            Rast_get_cellhd(this->GetRasterName(), this->GetMapset(), &head);
            G_set_window(&head);
            this->Region->CopyRegionFrom(&head);
        }
        else if (this->RegionUsage == VTK_GRASS_REGION_USER && this->Region != NULL)
        {
            this->Region->AdjustRegion();
            this->Region->CopyRegionTo(&head);
            G_set_window(&head);
        }
        else
        {
            // Use current region as default
            G_get_set_window(&head);
            this->Region->CopyRegionFrom(&head);
        }
    CATCH_BOOL

    this->NumberOfRows = head.rows;
    this->NumberOfCols = head.cols;

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapBase::SetUpRasterBuffer()
{

    // The region must be set and map must be open to allocate the buffer
    if (this->Open == false || this->Map < 0)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate raster buffer. Raster map is not open",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (this->Row == NULL)
    {
        if (this->MapType == CELL_TYPE)
            this->Row = vtkSmartPointer<vtkIntArray>::New();
        if (this->MapType == FCELL_TYPE)
            this->Row = vtkSmartPointer<vtkFloatArray>::New();
        if (this->MapType == DCELL_TYPE)
            this->Row = vtkSmartPointer<vtkDoubleArray>::New();

        this->Row->SetNumberOfTuples(this->NumberOfCols);
        this->MapType = Rast_get_map_type(this->Map);
    }

    if (this->RasterBuff == NULL)
    {
        this->RasterBuff = Rast_allocate_d_buf();
    }

    return true;
}

//----------------------------------------------------------------------------

vtkDataArray *
vtkGRASSRasterMapBase::GetRow(int idx)
{
    int i;
    char buff[1024];

    if (idx < 0 || idx > this->NumberOfCols - 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                   this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    this->SetUpRasterBuffer();
    TRY Rast_get_d_row(this->Map, this->RasterBuff, idx);
    CATCH_NULL

    for (i = 0; i < this->NumberOfCols; i++)
    {
        if(this->UseGRASSNulleValue)
        {
            if(Rast_is_d_null_value(&this->RasterBuff[i]))
                this->Row->SetTuple1(i, this->NullValue);
            else
                this->Row->SetTuple1(i, this->RasterBuff[i]);
        }else
        {
            this->Row->SetTuple1(i, this->RasterBuff[i]);
        }
    }

    return this->Row;
}

//----------------------------------------------------------------------------

vtkCharArray *
vtkGRASSRasterMapBase::GetNullRow(int idx)
{
    int i;
    char buff[1024];

    if (idx < 0 || idx > this->NumberOfCols - 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                   this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    this->SetUpRasterBuffer();
    TRY Rast_get_null_value_row(this->Map, this->NullBuff, idx);
    CATCH_NULL

    for (i = 0; i < this->NumberOfCols; i++)
    {
        this->NullRow->SetValue(i, this->NullBuff[i]);
    }

    return this->NullRow;
}

