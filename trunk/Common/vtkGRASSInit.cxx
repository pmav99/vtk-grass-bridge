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

extern "C"
{
#include <grass/gis.h>
#include <setjmp.h>
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
		vgb_error_message = msg;
        longjmp(vgb_stack_buffer, 1);
    }
    return 1;
}

//----------------------------------------------------------------------------

vtkGRASSInit::vtkGRASSInit()
{
    G_gisinit("vtkGRASSInit");
    // Set the error routine
    G_set_error_routine(vgb_error_handler);
}
