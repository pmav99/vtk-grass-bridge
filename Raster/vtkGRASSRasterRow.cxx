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

#include "vtkGRASSRasterRow.h"
#include "vtkGRASSRasterMapBase.h"
#include <vtkObjectFactory.h>

vtkCxxRevisionMacro(vtkGRASSRasterRow, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSRasterRow);

//----------------------------------------------------------------------------

vtkGRASSRasterRow::vtkGRASSRasterRow()
{
    this->Allocated = false;
    this->CELLBuff = NULL;
    this->FCELLBuff = NULL;
    this->DCELLBuff = NULL;
    this->NumberOfCols = 0;
    this->RowType = -1;
}

//----------------------------------------------------------------------------

vtkGRASSRasterRow::~vtkGRASSRasterRow()
{
    this->Reset();
}

//----------------------------------------------------------------------------

void vtkGRASSRasterRow::Reset()
{
    TRY
    if(this->CELLBuff)
        G_free(this->CELLBuff);
    if(this->FCELLBuff)
        G_free(this->FCELLBuff);
    if(this->DCELLBuff)
        G_free(this->DCELLBuff);
    CATCH_VOID

    this->NumberOfCols = 0;
    this->RowType = -1;
    this->Allocated = false;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::Allocate(int cols, int rowtype)
{
    if(rowtype == CELL_TYPE)
        return this->AllocateCELL(cols);
    if(rowtype == FCELL_TYPE)
        return this->AllocateFCELL(cols);
    if(rowtype == DCELL_TYPE)
        return this->AllocateDCELL(cols);

    return false;
}
//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::AllocateCELL(int cols)
{
    if(cols < 1) {
        return false;
        this->InsertNextError("Number of columns is lower than 1");
    }

    this->Reset();

    TRY
    this->CELLBuff = (CELL*)G_calloc(cols, sizeof(CELL));
    CATCH_BOOL

    this->NumberOfCols = cols;
    this->Allocated = true;
    this->RowType = CELL_TYPE;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::AllocateFCELL(int cols)
{
    if(cols < 1) {
        return false;
        this->InsertNextError("Number of columns is lower than 1");
    }

    this->Reset();

    TRY
    this->FCELLBuff = (FCELL*)G_calloc(cols, sizeof(FCELL));
    CATCH_BOOL

    this->NumberOfCols = cols;
    this->Allocated = true;
    this->RowType = FCELL_TYPE;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::AllocateDCELL(int cols)
{
    if(cols < 1) {
        return false;
        this->InsertNextError("Number of columns is lower than 1");
    }

    this->Reset();

    TRY
    this->DCELLBuff = (DCELL*)G_calloc(cols, sizeof(DCELL));
    CATCH_BOOL

    this->NumberOfCols = cols;
    this->Allocated = true;
    this->RowType = DCELL_TYPE;

    return true;
}

//----------------------------------------------------------------------------

void *vtkGRASSRasterRow::GetBuffer()
{
    if(this->Allocated == false)
        return NULL;

    if(this->RowType == CELL_TYPE)
        return this->CELLBuff;
    if(this->RowType == FCELL_TYPE)
        return this->FCELLBuff;
    if(this->RowType == DCELL_TYPE)
        return this->DCELLBuff;

    return NULL;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::GetCELLValue(int col, vtkCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    value->Value = this->CELLBuff[col];

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::GetFCELLValue(int col, vtkFCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    value->Value = this->FCELLBuff[col];

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::GetDCELLValue(int col, vtkDCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    value->Value = this->DCELLBuff[col];

    return true;
}


//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::SetCELLValue(int col, vtkCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    this->CELLBuff[col] = value->Value;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::SetFCELLValue(int col, vtkFCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    this->FCELLBuff[col] = value->Value;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::SetDCELLValue(int col, vtkDCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    this->DCELLBuff[col] = value->Value;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::IsNull(int col)
{
    if(this->Allocated == false)
        return false;

    if(this->RowType == CELL_TYPE)
        return (Rast_is_c_null_value(this->CELLBuff[col]?true:false));
    if(this->RowType == FCELL_TYPE)
        return (Rast_is_f_null_value(this->FCELLBuff[col]?true:false));
    if(this->RowType == DCELL_TYPE)
        return (Rast_is_d_null_value(this->DCELLBuff[col]?true:false));

    return false;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::SetToNull(int col)
{
    if(this->Allocated == false)
        return false;

    if(this->RowType == CELL_TYPE)
        Rast_set_c_null_value(&(this->CELLBuff[col]), 1);
    if(this->RowType == FCELL_TYPE)
        Rast_set_f_null_value(&(this->FCELLBuff[col]), 1);
    if(this->RowType == DCELL_TYPE)
        Rast_set_d_null_value(&(this->DCELLBuff[col]), 1);

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterRow::SetToNull()
{
    if(this->Allocated == false)
        return false;

    if(this->RowType == CELL_TYPE)
        Rast_set_c_null_value(this->CELLBuff, this->NumberOfCols);
    if(this->RowType == FCELL_TYPE)
        Rast_set_f_null_value(this->FCELLBuff, this->NumberOfCols);
    if(this->RowType == DCELL_TYPE)
        Rast_set_d_null_value(this->DCELLBuff, this->NumberOfCols);

    return true;
}

//----------------------------------------------------------------------------

vtkDataArray *vtkGRASSRasterRow::ToDataArray()
{
    if(this->Allocated == false)
        return NULL;

    return NULL;
}