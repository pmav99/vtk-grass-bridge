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

#include "vtkGRASSHistory.h"
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"

extern "C"
{
#include <string.h>
}

vtkCxxRevisionMacro(vtkGRASSHistory, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSHistory);

//----------------------------------------------------------------------------

vtkGRASSHistory::vtkGRASSHistory()
{

    Rast_short_history("Title", "raster", &this->hist);
    sprintf(this->hist.keywrd, "generated by %s", G_program_name());

    this->RasterName = NULL;

}

//----------------------------------------------------------------------------

vtkGRASSHistory::~vtkGRASSHistory()
{
    if (this->RasterName)
        delete [] this->RasterName;

}


//----------------------------------------------------------------------------

void
vtkGRASSHistory::SetTitle(const char *title)
{
    int size;
    size = strlen(title);

    if (size > 79)
        size = 79;

    G_snprintf(hist.title, size, "%s", title);
}

//----------------------------------------------------------------------------

void
vtkGRASSHistory::SetCreator(const char *creator)
{
    int size;
    size = strlen(creator);

    if (size > 79)
        size = 79;

    G_snprintf(hist.creator, size, "%s", creator);
}
//----------------------------------------------------------------------------

void
vtkGRASSHistory::SetDataSource1(const char *datsrc_1)
{
    int size;
    size = strlen(datsrc_1);

    if (size > 79)
        size = 79;

    G_snprintf(hist.datsrc_1, size, "%s", datsrc_1);
}
//----------------------------------------------------------------------------

void
vtkGRASSHistory::SetDataSource2(const char *datsrc_1)
{
    int size;
    size = strlen(datsrc_1);

    if (size > 79)
        size = 79;

    G_snprintf(hist.datsrc_1, size, "%s", datsrc_1);
}
//----------------------------------------------------------------------------

void
vtkGRASSHistory::SetKeywords(const char *keywrd)
{
    int size;
    size = strlen(keywrd);

    if (size > 79)
        size = 79;

    G_snprintf(hist.keywrd, size, "%s", keywrd);
}

//----------------------------------------------------------------------------

bool
vtkGRASSHistory::InsertNextCommentLine(const char *comment)
{
    int size;
    if(hist.edlinecnt > 49)
        hist.edlinecnt = 0;

    size = strlen(comment);

    if (size > 79)
        size = 79;

    G_snprintf(this->hist.edhist[hist.edlinecnt], size, "%s", comment);
    

    return true;
}

//----------------------------------------------------------------------------

int
vtkGRASSHistory::GetNumbeOfComments()
{
    return this->hist.edlinecnt;
}

//----------------------------------------------------------------------------

const char *
vtkGRASSHistory::GetComment(int idx)
{
    char buff[1024];
    
    if(idx < this->hist.edlinecnt)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Wrong index.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return NULL;
    }

    return this->hist.edhist[idx];
}


//----------------------------------------------------------------------------

bool
vtkGRASSHistory::ReadHistory(char *rastername)
{
    const char *mapset;
    char buff[1024];


    mapset = G_find_raster2(rastername, "");
    if (mapset == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to find map %s.",
                   this->GetClassName(), __LINE__, rastername);
        this->InsertNextError(buff);
        return false;
    }

    if (Rast_read_history(rastername, mapset, &this->hist) == -1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to read history file of map %s.",
                   this->GetClassName(), __LINE__, rastername);
        this->InsertNextError(buff);
        return false;
    }

    this->SetRasterName(rastername);

    return true;
}


//----------------------------------------------------------------------------

bool
vtkGRASSHistory::WriteHistory(char *rastername)
{
    char buff[1024];
    TRY
    if(Rast_write_history(rastername, &this->hist) == -1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to write history file of map %s.",
                   this->GetClassName(), __LINE__, rastername);
        this->InsertNextError(buff);
        return false;
    }
    CATCH_BOOL

    this->SetRasterName(rastername);
    
    return true;
}

//----------------------------------------------------------------------------

void
vtkGRASSHistory::PrintSelf(ostream& os, vtkIndent indent)
{
    int i;
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Raster name: " << (this->RasterName ? this->RasterName : "(none)") << endl;
    os << indent << "mapid: " << this->hist.mapid << endl;
    os << indent << "title: " << this->hist.title << endl;
    os << indent << "mapset: " << this->hist.mapset << endl;
    os << indent << "creator: " << this->hist.creator << endl;
    os << indent << "maptype: " << this->hist.maptype << endl;
    os << indent << "datsrc_1: " << this->hist.datsrc_1 << endl;
    os << indent << "datsrc_2: " << this->hist.datsrc_2 << endl;
    os << indent << "keywrd: " << this->hist.keywrd << endl;

    for(i = 0; i < this->hist.edlinecnt; i++)
        os << indent.GetNextIndent() << "Comment " << i << ": " << this->hist.edhist[i] << endl;

    return;
}
