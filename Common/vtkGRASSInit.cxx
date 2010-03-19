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

#include "vtkGRASSInit.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"
#include <vtkStdString.h>
#include <sstream>

extern "C"
{
#include <grass/gis.h>
#include <setjmp.h>
#include <c++/4.4/bits/basic_string.h>
}

vtkCxxRevisionMacro(vtkGRASSInit, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSInit);

threadLocal const char* vgb_error_message = NULL;
threadLocal jmp_buf vgb_stack_buffer;


//----------------------------------------------------------------------------

static int
vgb_error_handler(const char *msg, int fatal)
{
    if (fatal == 0)
    {
        fprintf(stderr, "%s\n", msg);
        return 1;
    }
    else 
    {
		fprintf(stderr, "\n############## Exceptiont called ###########\n");
		vgb_error_message = msg;
        longjmp(vgb_stack_buffer, 1);
    }
    return 1;
}

//----------------------------------------------------------------------------

vtkGRASSInit::vtkGRASSInit()
{
    // Set the error routine
    G_set_error_routine(vgb_error_handler);
	G_sleep_on_error(0);
}

//----------------------------------------------------------------------------

void vtkGRASSInit::ExitOnErrorOff()
{
    // Set the error routine
    G_set_error_routine(vgb_error_handler);

}

//----------------------------------------------------------------------------

void vtkGRASSInit::ExitOnErrorOn()
{
    // Set the error routine
    G_unset_error_routine();

}

//----------------------------------------------------------------------------

void vtkGRASSInit::Init(const char *name)
{
    // Init GRASS
    G_gisinit(name);
}

//----------------------------------------------------------------------------

void vtkGRASSInit::Parser(const char *argv)
{
	char **buff = NULL;
	int num = 0;
	vtkStdString a(argv), value;
	std::stringstream ss;

	// Count the number of arguments
	ss << a;
	while (ss >> value)
	{
		num++;
	}

	buff = (char **)G_calloc(num, sizeof(char*));

	// Put the arguments into the charachter array
	num = 0;
	ss << a;
	while (ss >> value)
	{
		buff[num] = (char*)G_calloc(value.size() + 1, sizeof(char));
		G_snprintf(buff[num], value.size(), "%s", value.c_str());
		num++;
	}

    // Start the grass command line parser
    G_parser(num, buff);
}

//----------------------------------------------------------------------------

void vtkGRASSInit::Parser(int argc, char **argv)
{
    // Start the grass command line parser
    G_parser(argc, argv);
}