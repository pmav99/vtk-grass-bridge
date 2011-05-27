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
    this->RasterBuff = (void*) NULL;
    this->NullBuff = (char*) NULL;
    this->NullValue = -999999;
    this->UseNullValue = 1;
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
    if (this->NullBuff)
        G_free(this->NullBuff);
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterMapBase::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Is map open: " << (this->Open ? "YES" : "NO") << endl;
    if (this->Open) {
        os << indent << "Name   : " << this->RasterName << endl;
        os << indent << "Mapset : " << this->Mapset << endl;
        os << indent << "Maptype: " << this->GetMapTypeAsString() << endl;
        os << indent << "Cols   : " << this->NumberOfCols << endl;
        os << indent << "Rows   : " << this->NumberOfRows << endl;
        os << indent << "UseNullValue: " << this->UseNullValue << endl;
        this->History->PrintSelf(os, indent.GetNextIndent());
    }
    return;
}

//----------------------------------------------------------------------------

const char* vtkGRASSRasterMapBase::GetMapTypeAsString()
{
    if (this->MapType == CELL_TYPE)
        return "CELL_TYPE";
    if (this->MapType == FCELL_TYPE)
        return "FCELL_TYPE";
    if (this->MapType == DCELL_TYPE)
        return "DCELL_TYPE";

    return NULL;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapBase::SetRegion()
{
    struct Cell_head head;

    if (this->RasterName == NULL || this->Mapset == NULL) {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to set the region. RasterName is not set.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }
    TRY
    if (this->RegionUsage == VTK_GRASS_REGION_CURRENT) {
        G_get_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    } else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT) {
        G_get_default_window(&head);
        G_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    } else if (this->RegionUsage == VTK_GRASS_REGION_USER && this->Region != NULL) {
        this->Region->CopyRegionTo(&head);
        G_set_window(&head);
    } else {
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
    if (this->Open == false || this->Map < 0) {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate raster buffer. Raster map is not open",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (this->Row == NULL) {
        if (this->MapType == CELL_TYPE)
            this->Row = vtkSmartPointer<vtkIntArray>::New();
        else if (this->MapType == FCELL_TYPE)
            this->Row = vtkSmartPointer<vtkFloatArray>::New();
        else if (this->MapType == DCELL_TYPE)
            this->Row = vtkSmartPointer<vtkDoubleArray>::New();

        this->Row->SetNumberOfTuples(this->NumberOfCols);
    }

    TRY
    if (this->RasterBuff == NULL) {
        if (this->MapType == CELL_TYPE)
            this->RasterBuff = (CELL*) G_calloc(this->NumberOfCols, sizeof (CELL));
        else if (this->MapType == FCELL_TYPE)
            this->RasterBuff = (FCELL*) G_calloc(this->NumberOfCols, sizeof (FCELL));
        else if (this->MapType == DCELL_TYPE)
            this->RasterBuff = (DCELL*) G_calloc(this->NumberOfCols, sizeof (DCELL));

    }
    CATCH_BOOL

    return true;
}

//----------------------------------------------------------------------------

vtkDataArray *
vtkGRASSRasterMapBase::GetRow(int idx)
{
    int i;
    void *ptr;
    char buff[1024];

    if (idx < 0 || idx > this->NumberOfRows - 1) {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                   this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    // Allocate the arster buffer with the map specific type
    this->SetUpRasterBuffer();

    TRY Rast_get_row(this->Map, this->RasterBuff, idx, this->MapType);
    CATCH_NULL

    ptr = this->RasterBuff;

    for (i = 0; i < this->NumberOfCols; i++) {

        if (this->UseNullValue) {
            if ((this->MapType == CELL_TYPE && Rast_is_c_null_value(&((CELL*)this->RasterBuff)[i])) ||
                (this->MapType == FCELL_TYPE && Rast_is_f_null_value(&((FCELL*)this->RasterBuff)[i])) ||
                (this->MapType == DCELL_TYPE && Rast_is_d_null_value(&((DCELL*)this->RasterBuff)[i]))) {
                this->Row->SetTuple1(i, this->NullValue);
            } else {

                if (this->MapType == CELL_TYPE) {
                    this->Row->SetTuple1(i, (double) ((CELL*)this->RasterBuff)[i]);
                } else if (this->MapType == FCELL_TYPE) {
                    this->Row->SetTuple1(i, (double) ((FCELL*)this->RasterBuff)[i]);
                } else if (this->MapType == DCELL_TYPE) {
                    this->Row->SetTuple1(i, (double) ((DCELL*)this->RasterBuff)[i]);
                }
            }
        } else {
            if (this->MapType == CELL_TYPE) {
                this->Row->SetTuple1(i, (double) ((CELL*)this->RasterBuff)[i]);
            } else if (this->MapType == FCELL_TYPE) {
                this->Row->SetTuple1(i, (double) ((FCELL*)this->RasterBuff)[i]);
            } else if (this->MapType == DCELL_TYPE) {
                this->Row->SetTuple1(i, (double) ((DCELL*)this->RasterBuff)[i]);

            }
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

    if (idx < 0 || idx > this->NumberOfRows - 1) {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                   this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    this->SetUpRasterBuffer();
    TRY Rast_get_null_value_row(this->Map, this->NullBuff, idx);
    CATCH_NULL

    for (i = 0; i < this->NumberOfCols; i++) {
        this->NullRow->SetValue(i, this->NullBuff[i]);
    }

    return this->NullRow;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapBase::CloseMap()
{
    // Cleaning up the null buffer for reuse
    if (this->NullBuff) {
        G_free(this->NullBuff);
        this->NullBuff = (char*) NULL;
    }

    // Cleaning up the raster buffer for reuse
    if (this->RasterBuff) {
        G_free(this->RasterBuff);
        this->RasterBuff = (void*) NULL;
    }

    if (this->Open == true && this->Map != -1) {
        TRY Rast_close(this->Map);
        CATCH_BOOL
    }
    // This flag is important and must be set by open and close methods
    this->Open = false;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetSampleValue(double x, double y, double &value, int type)
{
    if (!this->IsOpen()){
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to compute sample because map is not open.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }
    
    value = (double)Rast_get_sample(this->Map, this->Region->GetPointer(), NULL, x, y, 0, type);
        
    return !this->IsDoubleNullValue(value);
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetNearestSampleValue(double x, double y, double &value)
{
    return this->GetSampleValue(x, y, value, NEAREST);
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetBilinearSampleValue(double x, double y, double &value)
{
    return this->GetSampleValue(x, y, value, BILINEAR);
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetBicubicSampleValue(double x, double y, double &value)
{
    return this->GetSampleValue(x, y, value, CUBIC);
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::IsDoubleNullValue(double value)
{
    if (Rast_is_d_null_value(&value))
        return true;
    else
        return false;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::IsFloatNullValue(float value)
{
    if (Rast_is_f_null_value(&value))
        return true;
    else
        return false;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::IsIntegerNullValue(int value)
{
    if (Rast_is_c_null_value(&value))
        return true;
    else
        return false;
}