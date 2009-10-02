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

#include "vtkGRASSDatabaseInfo.h"
#include <vtkObjectFactory.h>

extern "C" {
#include <grass/gis.h>
#include <math.h>
}

vtkCxxRevisionMacro(vtkGRASSDatabaseInfo, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSDatabaseInfo);

//----------------------------------------------------------------------------

vtkGRASSDatabaseInfo::vtkGRASSDatabaseInfo() {

    this->CurrentLocationName = NULL;
    this->CurrentLocationPath = NULL;
    this->CurrentLocationTitle = NULL;
    this->CurrentMapsetName = NULL;
    this->GisBasePath = NULL;

    this->AvailableMapSets = vtkStringArray::New();

    //G_gisinit("vtkGRASSDatabaseInfo");

    this->Refresh();
}

//----------------------------------------------------------------------------

vtkGRASSDatabaseInfo::~vtkGRASSDatabaseInfo() {

    if(this->CurrentLocationName)
        delete [] this->CurrentLocationName;
    if(this->CurrentLocationPath)
        delete [] this->CurrentLocationPath;
    if(this->CurrentLocationTitle)
        delete [] this->CurrentLocationTitle;
    if(this->CurrentMapsetName)
        delete [] this->CurrentMapsetName;
    if(this->GisBasePath)
        delete [] this->GisBasePath;

    this->AvailableMapSets->Delete();

}
//----------------------------------------------------------------------------

bool vtkGRASSDatabaseInfo::Refresh(){

  this->AvailableMapSets->Initialize();

  this->SetCurrentLocationName(G_location());
  this->SetCurrentMapsetName(G_mapset());
  this->SetCurrentLocationTitle(G_myname());
  this->SetGisBasePath(G_gisbase());
  this->SetCurrentLocationPath(G_location_path());

  char ** mapsets = G_available_mapsets();
  
  int count = 0;
  while(mapsets && mapsets[count])
  {
   this->AvailableMapSets->InsertNextValue(mapsets[count]);
   count++;
  }

  return true;

}

//----------------------------------------------------------------------------

void vtkGRASSDatabaseInfo::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);

    os << indent << "CurrentLocationName: " << (this->CurrentLocationName?this->CurrentLocationName : "none") << endl;
    os << indent << "CurrentLocationPath: " << (this->CurrentLocationPath?this->CurrentLocationPath : "none") << endl;
    os << indent << "CurrentLocationTitle: " << (this->CurrentLocationTitle?this->CurrentLocationTitle : "none") << endl;
    os << indent << "CurrentMapsetName: " << (this->CurrentMapsetName?this->CurrentMapsetName : "none") << endl;
    os << indent << "GisBasePath: " << (this->GisBasePath?this->GisBasePath : "none") << endl;

    os << indent << "Available Mapsets: " << endl;
    int i;
    for(i = 0; i < this->AvailableMapSets->GetNumberOfValues(); i++)
    {
        os << indent.GetNextIndent() << this->AvailableMapSets->GetValue(i) << endl;
    }
}
