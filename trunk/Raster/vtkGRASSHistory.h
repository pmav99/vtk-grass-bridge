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
#include "vtkGRASSDefines.h"

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
  const char *GetMapId(){return Rast_get_history(this->hist, HIST_MAPID);}
  const char *GetTitle(){return Rast_get_history(this->hist, HIST_TITLE);}
  const char *GetMapset(){return Rast_get_history(this->hist, HIST_MAPSET);}
  const char *GetCreator(){return Rast_get_history(this->hist, HIST_CREATOR);}
  const char *GetMaptype(){return Rast_get_history(this->hist, HIST_MAPTYPE);}
  const char *GetDataSource1(){return Rast_get_history(this->hist, HIST_DATSRC_1);}
  const char *GetDataSource2(){return Rast_get_history(this->hist, HIST_DATSRC_2);}
  const char *GetKeywords(){return Rast_get_history(this->hist, HIST_KEYWRD);}

  void SetTitle(const char *title){TRY Rast_set_history(this->hist, HIST_TITLE, title); CATCH_VOID}
  void SetCreator(const char *creator){TRY Rast_set_history(this->hist, HIST_CREATOR, creator); CATCH_VOID}
  void SetDataSource1(const char *datsrc_1){TRY Rast_set_history(this->hist, HIST_DATSRC_1, datsrc_1); CATCH_VOID}
  void SetDataSource2(const char *datsrc_2){TRY Rast_set_history(this->hist, HIST_DATSRC_2, datsrc_2); CATCH_VOID}
  void SetKeywords(const char *keywrd){TRY Rast_set_history(this->hist, HIST_KEYWRD, keywrd); CATCH_VOID}
  
  //! \brief Append a comment to the raster map history file.
  void  AppendComment(char *comment) {TRY Rast_append_history(this->hist, comment); CATCH_VOID}

  //BTX
  virtual struct History* GetPointer(){return this->hist;}
  //ETX

protected:
  vtkGRASSHistory();
  ~vtkGRASSHistory();

  vtkSetStringMacro(RasterName);
  vtkSetStringMacro(Mapset);

  char *RasterName;
  char *Mapset;

  //BTX
  struct History *hist;
  //ETX

private:
  vtkGRASSHistory(const vtkGRASSHistory&);  // Not implemented.
  void operator=(const vtkGRASSHistory&);  // Not implemented.
};


#endif
