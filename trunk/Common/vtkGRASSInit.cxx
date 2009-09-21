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
#include "vtkGRASSInit.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>


extern "C" {
#include <grass/gis.h>
#include <grass/Vect.h>
#include <math.h>
}

vtkCxxRevisionMacro(vtkGRASSInit, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSInit);


//----------------------------------------------------------------------------

static int error_handler(const char *msg, int fatal)
{
    fprintf(stderr, "%s", msg);

    return 0;
}

//----------------------------------------------------------------------------

 vtkGRASSInit::vtkGRASSInit() {

     G_gisinit("vtkGRASSInit");
     Vect_set_fatal_error(GV_FATAL_PRINT);
     G_set_error_routine(error_handler);
}
