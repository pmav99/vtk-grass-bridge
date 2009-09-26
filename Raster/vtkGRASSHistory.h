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


/**
 * \brief This class creates, reads and writes the History file of a raster map
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSHistory_h
#define __vtkGRASSHistory_h

#include "vtkGRASSBridgeRasterWin32Header.h"
#include "vtkObjectGRASSErrorHandler.h"

extern "C" {
#include <grass/gis.h>
#include <grass/raster.h>
}

class VTK_GRASS_BRIDGE_RASTER_EXPORT vtkGRASSHistory : public vtkObjectGRASSErrorHandler
{
public:
  static vtkGRASSHistory *New();
  vtkTypeRevisionMacro(vtkGRASSHistory,vtkObjectGRASSErrorHandler);
  void PrintSelf(ostream& os, vtkIndent indent);

  //! \brief Read the history of a raster map
  bool ReadHistory(char *rastername);
  //! \brief Write the history of a raster map
  bool WriteHistory(char *rastername);

  vtkGetStringMacro(RasterName);
  char *GetMapId(){return this->hist.mapid;}
  char *GetTitle(){return this->hist.title;}
  char *GetMapset(){return this->hist.mapset;}
  char *GetCreator(){return this->hist.creator;}
  char *GetMaptype(){return this->hist.maptype;}
  char *GetDataSource1(){return this->hist.datsrc_1;}
  char *GetDataSource2(){return this->hist.datsrc_2;}
  char *GetKeywords(){return this->hist.keywrd;}

  void SetTitle(const char *title);
  void SetCreator(const char *creator);
  void SetDataSource1(const char *datsrc_1);
  void SetDataSource2(const char *datsrc_2);
  void SetKeywords(const char *keywrd);
  
  //! \brief Adds a comment to the raster map history file.
  bool InsertNextCommentLine(const char *comment);
  //! \brief Get the number of Comments in the raster map histrory file. The maxium number is 50.
  int GetNumbeOfComments();
  //! \brief Get a comment of index idx from the raster map history file.
  const char *GetComment(int idx);

  //BTX
  virtual struct History* GetPointer(){return &this->hist;}
  //ETX

protected:
  vtkGRASSHistory();
  ~vtkGRASSHistory();

  vtkSetStringMacro(RasterName);
  vtkSetStringMacro(Mapset);

  char *RasterName;
  char *Mapset;

  //BTX
  struct History hist;
  //ETX

private:
  vtkGRASSHistory(const vtkGRASSHistory&);  // Not implemented.
  void operator=(const vtkGRASSHistory&);  // Not implemented.
};


#endif
