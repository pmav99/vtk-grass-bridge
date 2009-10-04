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


/** Global preprocessor definitions for vtkGRASSBridge
*/
#define VTK_GRASS_REGION_DEFAULT 0
#define VTK_GRASS_REGION_CURRENT 1
#define VTK_GRASS_REGION_RASTER  2
#define VTK_GRASS_REGION_USER    3


/*Thread local and setjmp() exception support*/
extern "C" {
#include <grass/gis.h>
#include <setjmp.h>
}

#ifdef WIN32
#define Thread   __declspec( thread )
#else
#define threadLocal   __thread
#endif

#define TRY         if (!setjmp(vgb_stack_buffer)) {

#define CATCH_INT   } else { \
                    this->InsertNextError(vgb_error_message); \
                    return -1;}

#define CATCH_VINT (int num)  } else { \
                    this->InsertNextError(vgb_error_message); \
                    return num;}

#define CATCH_VOID  } else { \
                    this->InsertNextError(vgb_error_message); \
                    return;}

#define CATCH_BOOL  } else { \
                    this->InsertNextError(vgb_error_message); \
                    return false;}

extern threadLocal jmp_buf vgb_stack_buffer;    /*to save the most important CPU register for each thread*/
extern threadLocal const char* vgb_error_message;
