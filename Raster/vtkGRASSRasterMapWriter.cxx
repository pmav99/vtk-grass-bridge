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

#include "vtkGRASSRasterMapWriter.h"
#include "vtkGRASSHistory.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>

vtkCxxRevisionMacro(vtkGRASSRasterMapWriter, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSRasterMapWriter);

//----------------------------------------------------------------------------

vtkGRASSRasterMapWriter::vtkGRASSRasterMapWriter()
{
    this->RowCount = 0;
    this->MapTitle = NULL;
    this->SetMapTitle("Raster map title");
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapWriter::OpenMap(char *name)
{
    char buff[1024];
    int error = 0;

    // Check if the same map is already opened
    if (this->Open == true && strcmp(name, this->RasterName) == 0)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Raster map %s is already open.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->InsertNextError(buff);
        return false;
    }
    else if (this->Open == true)
    {
        // If a new name is given, the open map will be closed
        this->CloseMap();
    }

    this->SetRasterName(name);


    if (G_legal_filename(this->RasterName) != 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Raster map name %s is not valid.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->InsertNextError(buff);
        return false;
    }

    this->SetMapset(G_mapset());

    // Set the region for the map
    this->SetRegion();

    if (!setjmp(vgb_stack_buffer))
    {
        /* open raster map */
        this->Map = Rast_open_new(this->RasterName, this->MapType);
        if (this->Map < 0)
        {
            error = 1;
        }
    }
    else
    {
        this->InsertNextError(vgb_error_message);
        this->Open = false;
        this->RowCount = 0;
        return false;	
    }

    if (error == 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to open new raster map %s.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->Open = false;
        this->RowCount = 0;
        this->InsertNextError(buff);
        return false;
    }

    this->RowCount = 0;
    this->Open = true;
    return true;
}

//----------------------------------------------------------------------------

int
vtkGRASSRasterMapWriter::PutNextRow(vtkDataArray *data)
{

    int i;
    int ret;
    char buff[1024];
    int error = 0;

    if (data == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The data array is NULL.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return -1;
    }

    if (data->GetNumberOfTuples() < this->NumberOfCols)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The index of data is out of range.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return -1;
    }

    if (!this->SetUpRasterBuffer())
        return -1;

    // copy data
    for (i = 0; i < this->NumberOfCols; i++)
    {
        if (this->MapType == CELL_TYPE)
            ((CELL*)this->RasterBuff)[i] = (CELL) data->GetTuple1(i);
        if (this->MapType == FCELL_TYPE)
            ((FCELL*)this->RasterBuff)[i] = (FCELL) data->GetTuple1(i);
        if (this->MapType == DCELL_TYPE)
            ((DCELL*)this->RasterBuff)[i] = (DCELL) data->GetTuple1(i);
    }

    if (!setjmp(vgb_stack_buffer))
    {
        this->RowCount = Rast_put_row(this->Map, this->RasterBuff, this->MapType);
        if (this->RowCount < 0)
        {
            error = 1;
        }
    }
    else
    {
        this->InsertNextError(vgb_error_message);
        return -1;
    }

    if (error == 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to put row %i.",
                   this->GetClassName(), __LINE__, ret);
        this->InsertNextError(buff);
        return -1;
    }

    return this->RowCount;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapWriter::CloseMap()
{
    int error = 0;

    if (this->Open == true && this->Map != -1)
    {
        if (!setjmp(vgb_stack_buffer))
        {
            if (Rast_close(this->Map) != 1)
            {
                error = 1;
            }
        }
        else
        {
            this->InsertNextError(vgb_error_message);
            return false;
        }

        if (error == 1)
        {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to close raster map %s.",
                       this->GetClassName(), __LINE__, this->RasterName);
            this->InsertNextError(buff);
            return false;
        }
    }

    // This flag is important and must be set by open and close methods
    this->Open = false;

    // Cleaning up the raster buffer for reuse
    if (this->RasterBuff)
    {
        G_free(this->RasterBuff);
        this->RasterBuff = (void*) NULL;
    }
    // Cleaning up the vtkDataArray for reuse
    if (this->Row)
    {
        this->Row->Delete();
        this->Row = NULL;
    }

    if (!this->History->WriteHistory(this->RasterName))
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i %s.",
                   this->GetClassName(), __LINE__, this->History->GetError());
        this->InsertNextError(buff);
        return false;
    }


    return true;
}
