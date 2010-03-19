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
 * \brief
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 *
 * \TODO
 *
 * */

#ifndef __vtkGRASSOption_h
#define __vtkGRASSOption_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"
{
#include <grass/gis.h>
}

#define VGB_CHECK_OPTION if(this->option == NULL){ this->option = G_define_option();}

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSOption : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSOption *New();
  vtkTypeRevisionMacro(vtkGRASSOption,vtkObjectGRASSErrorHandler);

  //!\brief Set the key.
  //! This will set the identifier string
  void SetKey(char *key){VGB_CHECK_OPTION this->option->key = key;}
  //!\brief Set the description of the option
  //! This description will be used within the help and the manual page
  void SetDescription(const char *description){VGB_CHECK_OPTION this->option->description = description;}
  //!\brief Set the label of the module
  void SetLabel(const char *label){VGB_CHECK_OPTION this->option->label = label;}
  //!\brief Set the gui section identifier. This string is used to create
  //! specific tabs in the generated gui
  void SetGuiSection(const char *guisection){VGB_CHECK_OPTION this->option->guisection = guisection;}
  //!\brief Set the gispompt string ("old,raster,cell" or "new,vector,vector")
  void SetGisprompt(char *gisprompt){VGB_CHECK_OPTION this->option->gisprompt = gisprompt;}
  //!\brief Set default options ("a,b,c,d")
  void SetDefaultOptions(const char *options) {VGB_CHECK_OPTION this->option->options = options;}
  //!\brief Set the default answer
  void SetDefaultAnswer(char *answer) {VGB_CHECK_OPTION this->option->answer = answer;}

  //!\brief Set if the option support multiple answers
  void MultipleOn() {VGB_CHECK_OPTION this->option->multiple = 1;}
  //!\brief Set if the option support multiple answers
  void MultipleOff(){VGB_CHECK_OPTION this->option->multiple = 0;}
  //!\brief Set if the option is required
  void RequiredOn() {VGB_CHECK_OPTION this->option->required = 1;}
  //!\brief Set if the option is required
  void RequiredOff(){VGB_CHECK_OPTION this->option->required = 0;}
  //!\brief Set the type of the option to integer
  void SetTypeToInteger(){VGB_CHECK_OPTION this->option->type = TYPE_INTEGER;}
  //!\brief Set the type of the option to double
  void SetTypeToDouble() {VGB_CHECK_OPTION this->option->type = TYPE_DOUBLE;}
  //!\brief Set the type of the option to string
  void SetTypeToString() {VGB_CHECK_OPTION this->option->type = TYPE_STRING;}
  
  //!\brief Return the option desription
  const char * GetDescription(){VGB_CHECK_OPTION return this->option->description;}
  //!\brief Return the guisection string
  const char* GetGuiSection() {VGB_CHECK_OPTION return this->option->guisection;}
  //!\brief Return the label string
  const char* GetLabel() {VGB_CHECK_OPTION return this->option->label;}
  //!\brief Return the gisprompt string
  const char* GetGisprompt() {VGB_CHECK_OPTION return this->option->gisprompt;}
  //!\brief Return the answer string
  const char* GetAnswer() {VGB_CHECK_OPTION return this->option->answer;}
  //!\brief Return the options string
  const char* GetOptions() {VGB_CHECK_OPTION return this->option->options;}
  //!\brief Check if the option is multiple (true/false)
  bool IsMultiple() {VGB_CHECK_OPTION return (this->option->multiple?true:false);}
  //!\brief Check if the option is required (true/false)
  bool IsRequired() {VGB_CHECK_OPTION return (this->option->required?true:false);}
  //!\brief Check if the type is integer (true/false)
  bool IsTypeInteger() {VGB_CHECK_OPTION return (this->option->type == TYPE_INTEGER?true:false);}
  //!\brief Check if the type is double (true/false)
  bool IsTypeDouble() {VGB_CHECK_OPTION return (this->option->type == TYPE_DOUBLE ?true:false);}
  //!\brief Check if the type is string (true/false)
  bool IsTypeString() {VGB_CHECK_OPTION return (this->option->type == TYPE_STRING?true:false);}

  //!\brief Create a standard option: "WHERE conditions of SQL statement without 'where' keyword"
  void CreateOption_DB_Where(){this->option = G_define_standard_option(G_OPT_DB_WHERE);}
  //!\brief Create a standard option: Table name
  void CreateOption_DB_TABLE(){this->option = G_define_standard_option(G_OPT_DB_TABLE);}
  //!\brief Create a standard option: Driver name
  void CreateOption_DB_DRIVER(){this->option = G_define_standard_option(G_OPT_DB_DRIVER);}
  //!\brief Create a standard option: Database name
  void CreateOption_DB_DATABASE(){this->option = G_define_standard_option(G_OPT_DB_DATABASE);}
  //!\brief Create a standard option: Database schema
  void CreateOption_DB_SCHEMA(){this->option = G_define_standard_option(G_OPT_DB_SCHEMA);}
  //!\brief Create a standard option: attribute column
  void CreateOption_DB_COLUMN(){this->option = G_define_standard_option(G_OPT_DB_COLUMN);}
  //!\brief Create a standard option: attribute column(s)
  void CreateOption_DB_COLUMNS(){this->option = G_define_standard_option(G_OPT_DB_COLUMNS);}
  //!\brief Create a standard option: input raster map
  void CreateOption_R_INPUT(){this->option = G_define_standard_option(G_OPT_R_INPUT);}
  //!\brief Create a standard option: input raster map(s)
  void CreateOption_R_INPUTS(){this->option = G_define_standard_option(G_OPT_R_INPUTS);}
  //!\brief Create a standard option: output raster map
  void CreateOption_R_OUTPUT(){this->option = G_define_standard_option(G_OPT_R_OUTPUT);}
  //!\brief Create a standard option: input vector map
  void CreateOption_V_INPUT(){this->option = G_define_standard_option(G_OPT_V_INPUT);}
  //!\brief Create a standard option: input vector map(s)
  void CreateOption_V_INPUTS(){this->option = G_define_standard_option(G_OPT_V_INPUTS);}
  //!\brief Create a standard option: output vector map
  void CreateOption_V_OUTPUT(){this->option = G_define_standard_option(G_OPT_V_OUTPUT);}
  //!\brief Create a standard option: Feature type
  void CreateOption_V_TYPE(){this->option = G_define_standard_option(G_OPT_V_TYPE);}
  //!\brief Create a standard option: number or name
  void CreateOption_V_FIELD(){this->option = G_define_standard_option(G_OPT_V_FIELD);}
  //!\brief Create a standard option: input file
  void CreateOption_F_INPUT(){this->option = G_define_standard_option(G_OPT_F_INPUT);}
  //!\brief Create a standard option: output file
  void CreateOption_F_OUTPUT(){this->option = G_define_standard_option(G_OPT_F_OUTPUT);}

protected:
  vtkGRASSOption();
  ~vtkGRASSOption() {};

  //BTX
  struct Option *option;
  //ETX

private:
  vtkGRASSOption(const vtkGRASSOption&);  // Not implemented.
  void operator=(const vtkGRASSOption&);  // Not implemented.
};


#endif
