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
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSHistory.h"

vtkCxxRevisionMacro(vtkGRASSRasterMapBase, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSRasterMapBase);

//----------------------------------------------------------------------------

vtkGRASSRasterMapBase::vtkGRASSRasterMapBase() {
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
    this->RegionUsage = VTK_GRASS_REGION_CURRENT;
    this->RasterBuff = (void*) NULL;
}

//----------------------------------------------------------------------------

vtkGRASSRasterMapBase::~vtkGRASSRasterMapBase() {


    this->CloseMap();

    if (this->RasterName)
        delete [] this->RasterName;
    if (this->Mapset)
        delete [] this->Mapset;

    this->Region->Delete();
    this->History->Delete();

    if (this->Row)
        this->Row->Delete();

    if (this->RasterBuff)
        G_free(this->RasterBuff);
}


//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::SetRegion() {
    struct Cell_head head;

    if(this->RasterName == NULL || this->Mapset == NULL)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to set the region. RasterName is not set.",
                    this->GetClassName(), __LINE__);
        return false;
    }

    if (this->RegionUsage == VTK_GRASS_REGION_CURRENT) {
        G_get_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    } else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT) {
        G_get_default_window(&head);
        G_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    } else if (this->RegionUsage == VTK_GRASS_REGION_RASTER) {
        Rast_get_cellhd(this->GetRasterName(), this->GetMapset(), &head);
        G_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    } else if (this->RegionUsage == VTK_GRASS_REGION_USER && this->Region != NULL) {
        this->Region->AdjustRegion();
        this->Region->CopyRegionTo(&head);
        G_set_window(&head);
    } else {
        // Use current region as default
        G_get_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    }

    this->NumberOfRows = head.rows;
    this->NumberOfCols = head.cols;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::SetUpRasterBuffer() {

    // The region must be set and map must be open to allocate the buffer
    if(this->Open == false || this->Map < 0)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate raster buffer. Raster map is not open",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }
    
    if (this->Row == NULL) {
        if (this->MapType == CELL_TYPE)
            this->Row = vtkIntArray::New();
        if (this->MapType == FCELL_TYPE)
            this->Row = vtkFloatArray::New();
        if (this->MapType == DCELL_TYPE)
            this->Row = vtkDoubleArray::New();

        this->Row->SetNumberOfTuples(this->NumberOfCols);
        this->MapType = Rast_get_map_type(this->Map);
    }

    if (this->RasterBuff == NULL)
    {
        this->RasterBuff = Rast_allocate_buf(this->MapType);
    }

    return true;
}


//----------------------------------------------------------------------------

vtkDataArray *vtkGRASSRasterMapBase::GetRow(int idx) {

    void *ptr;
    int i;
    int size;
    char buff[1024];

    if (idx < 0 || idx > this->NumberOfCols - 1) {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    this->SetUpRasterBuffer();

    if (Rast_get_row(this->Map, this->RasterBuff, idx, this->MapType) < 0) {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to read row %i.",
                this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    // copy data
    size = Rast_cell_size(this->MapType);
    ptr = this->RasterBuff;
    
    if (this->MapType == CELL_TYPE)
        for (i = 0; i < this->NumberOfCols; i++, ptr = G_incr_void_ptr(ptr, size)){
            this->Row->SetTuple1(i, (double) *(CELL*) ptr);
        }
    if (this->MapType == FCELL_TYPE)
        for (i = 0; i < this->NumberOfCols; i++, ptr = G_incr_void_ptr(ptr, size)){
            this->Row->SetTuple1(i, (double) *(FCELL*) ptr);
        }
    if (this->MapType == DCELL_TYPE)
        for (i = 0; i < this->NumberOfCols; i++, ptr = G_incr_void_ptr(ptr, size)){
            this->Row->SetTuple1(i, (double) *(DCELL*) ptr);
        }
   

    return this->Row;
}
