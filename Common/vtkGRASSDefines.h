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


#include "vtkSmartPointer.h"

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
#elif linux
#define threadLocal   __thread
#else
//no thread local support
#endif

#define TRY         if (!setjmp(vgb_stack_buffer)) {

#define CATCH_INT   } else { \
                    char __buff[1024]; \
                    G_snprintf(__buff, 1024, "class: %s line: %i : %s", \
                    this->GetClassName(), __LINE__, vgb_error_message); \
                    this->InsertNextError(__buff); \
                    return -1;}

#define CATCH_VINT (int num)  } else { \
                    char __buff[1024]; \
                    G_snprintf(__buff, 1024, "class: %s line: %i : %s", \
                    this->GetClassName(), __LINE__, vgb_error_message); \
                    this->InsertNextError(__buff); \
                    return num;}

#define CATCH_VOID  } else { \
                    char __buff[1024]; \
                    G_snprintf(__buff, 1024, "class: %s line: %i : %s", \
                    this->GetClassName(), __LINE__, vgb_error_message); \
                    this->InsertNextError(__buff); \
                    return;}

#define CATCH_BOOL  } else { \
                    char __buff[1024]; \
                    G_snprintf(__buff, 1024, "class: %s line: %i : %s", \
                    this->GetClassName(), __LINE__, vgb_error_message); \
                    this->InsertNextError(__buff); \
                    return false;}

#define CATCH_NULL  } else { \
                    char __buff[1024]; \
                    G_snprintf(__buff, 1024, "class: %s line: %i : %s", \
                    this->GetClassName(), __LINE__, vgb_error_message); \
                    this->InsertNextError(__buff); \
                    return NULL;}



#define VGB_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


#ifdef threadLocal
extern threadLocal jmp_buf vgb_stack_buffer;    /*to save the most important CPU register for each thread*/
extern threadLocal const char* vgb_error_message;
#else
extern jmp_buf vgb_stack_buffer;    /*to save the most important CPU register*/
extern const char* vgb_error_message;
#endif