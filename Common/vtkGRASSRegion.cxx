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

#include "vtkGRASSRegion.h"
#include <vtkObjectFactory.h>

vtkCxxRevisionMacro(vtkGRASSRegion, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSRegion);

//----------------------------------------------------------------------------

vtkGRASSRegion::vtkGRASSRegion() {
    this->BytesPerCell = 0;
    this->CompressionFlag = 0;
    this->Rows = 0;
    this->Cols = 0;
    this->Projection = 0;
    this->Zone = 0;
    this->EastWestResolution = 0;
    this->NorthSouthResolution = 0;
    this->NorthernEdge = 0;
    this->SouthernEdge = 0;
    this->EasternEdge = 0;
    this->WesternEdge = 0;

    this->Name = NULL;

    this->ReadCurrentRegion();
}

//----------------------------------------------------------------------------

vtkGRASSRegion::~vtkGRASSRegion() {

    if(this->Name)
        delete [] this->Name;
}
//----------------------------------------------------------------------------

bool vtkGRASSRegion::ReadCurrentRegion() {
    G_get_set_window(&this->head);
    this->CopyRegionFrom(&this->head);
    this->Modified();
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::ReadDefaultRegion() {
    G_get_default_window(&this->head);
    this->CopyRegionFrom(&this->head);
    this->Modified();
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::ReadRegion(char *regionName) {

    char * mapset;
    char buff[1024];

    this->SetName(regionName);

    if (this->Name == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Please set the region name first",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (G_legal_filename(this->Name) < 0) {
        G_snprintf(buff, 1024, "class: %s line: %i Region name does not fit GRASS naming convecntion",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    mapset = G_find_file("windows", this->Name, "");
    if (!mapset) {
        G_snprintf(buff, 1024, "class: %s line: %i Region file <%s> not found",
                    this->GetClassName(), __LINE__, this->Name);
        this->InsertNextError(buff);
        return false;
    }
    if (G__get_window(&this->head, "windows", this->Name, mapset) != NULL) {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to read region <%s> in <%s>",
                    this->GetClassName(), __LINE__, this->Name,
                mapset);
        this->InsertNextError(buff);
        return false;
    }

    this->CopyRegionFrom(&this->head);
    this->Modified();

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::SaveRegion(char *regionName) {

    char *message;
    char buff[1024];

    this->SetName(regionName);
    this->CopyRegionTo(&this->head);
    
    if (this->Name == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Please set the region name first",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (G_legal_filename(this->Name) < 0) {
        G_snprintf(buff, 1024, "class: %s line: %i Region name does not fit GRASS naming convecntion",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    message = G_adjust_Cell_head(&this->head, 1, 1);
    if(message != NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i %s",
                    this->GetClassName(), __LINE__, message);
        this->InsertNextError(buff);
        return false;
    }

    this->CopyRegionTo(&this->head);
    if(G__put_window(&this->head, "windows", this->Name) != 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to save the region",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::SaveRegionAsDefault() {
    char *message;
    char buff[1024];

    this->CopyRegionTo(&this->head);

    message = G_adjust_Cell_head(&this->head, 1, 1);
    if(message != NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i %s",
                    this->GetClassName(), __LINE__, message);
        this->InsertNextError(buff);
        return false;
    }

    if(G_put_window(&this->head) != 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Error while writing default region",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::AdjustRegion() {
    char *message;
    char buff[1024];

    this->CopyRegionTo(&this->head);

    message = G_adjust_Cell_head(&this->head, 1, 1);
    if(message != NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i %s",
                    this->GetClassName(), __LINE__, message);
        this->InsertNextError(buff);
        return false;
    }

    this->CopyRegionFrom(&this->head);

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::CopyRegionFrom(struct Cell_head *head) {
    this->BytesPerCell = head->format;
    this->CompressionFlag = head->compressed;
    this->Rows = head->rows;
    this->Cols = head->cols;
    this->Projection = head->proj;
    this->Zone = head->zone;
    this->EastWestResolution = head->ew_res;
    this->NorthSouthResolution = head->ns_res;
    this->NorthernEdge = head->north;
    this->SouthernEdge = head->south;
    this->EasternEdge = head->east;
    this->WesternEdge = head->west;
    this->Modified();
    return true;
}
//----------------------------------------------------------------------------

bool vtkGRASSRegion::CopyRegionTo(struct Cell_head *head) {
    head->format = this->BytesPerCell;
    head->compressed = this->CompressionFlag;
    head->rows = this->Rows;
    head->cols = this->Cols;
    head->proj = this->Projection;
    head->zone = this->Zone;
    head->ew_res = this->EastWestResolution;
    head->ns_res = this->NorthSouthResolution;
    head->north = this->NorthernEdge;
    head->south = this->SouthernEdge;
    head->east = this->EasternEdge;
    head->west = this->WesternEdge;

    return true;
}

//----------------------------------------------------------------------------

void vtkGRASSRegion::DeepCopy(vtkGRASSRegion *region) {
    this->BytesPerCell = region->BytesPerCell;
    this->CompressionFlag = region->CompressionFlag;
    this->Rows = region->Rows;
    this->Cols = region->Cols;
    this->Projection = region->Projection;
    this->Zone = region->Zone;
    this->EastWestResolution = region->EastWestResolution;
    this->NorthSouthResolution = region->NorthSouthResolution;
    this->NorthernEdge = region->NorthernEdge;
    this->SouthernEdge = region->SouthernEdge;
    this->EasternEdge = region->EasternEdge;
    this->WesternEdge = region->WesternEdge;
    this->CopyRegionTo(&this->head);
    this->Modified();
    return;
}

//----------------------------------------------------------------------------

void vtkGRASSRegion::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Region name: " << (this->Name?this->Name:"(none)") << endl;
    os << indent << "BytesPerCell: " << this->BytesPerCell << endl;
    os << indent << "CompressionFlag: " << this->CompressionFlag << endl;
    os << indent << "Rows: " << this->Rows << endl;
    os << indent << "Cols: " << this->Cols << endl;
    os << indent << "Projection: " << this->Projection << endl;
    os << indent << "Zone: " << this->Zone << endl;
    os << indent << "EastWestResolution: " << this->EastWestResolution << endl;
    os << indent << "NorthSouthResolution: " << this->NorthSouthResolution << endl;
    os << indent << "NorthernEdge: " << this->NorthernEdge << endl;
    os << indent << "SouthernEdge: " << this->SouthernEdge << endl;
    os << indent << "EasternEdge: " << this->EasternEdge << endl;
    os << indent << "WesternEdge: " << this->WesternEdge << endl;
    return;
}
