/*=========================================================================
 FPla library
 Authors: Soeren Gebbert
 Created: Aug. 2009


 =========================================================================*/
/// \brief manage Windows system differences
/// 
/// The Header captures some system differences between Unix
/// and Windows operating systems. 

#ifndef __vtkGRASSBridgeFilteringWin32Header_h
#define __vtkGRASSBridgeFilteringWin32Header_h

#include <vtkGRASSBridgeConfigure.h>

#if defined(WIN32) && !defined(VTK_GRASS_BRIDGE_STATIC)
#if defined(vtkGRASSBridgeFiltering_EXPORTS)
#define VTK_GRASS_BRIDGE_FILTERING_EXPORT __declspec( dllexport ) 
#else
#define VTK_GRASS_BRIDGE_FILTERING_EXPORT __declspec( dllimport ) 
#endif
#else
#define VTK_GRASS_BRIDGE_FILTERING_EXPORT
#endif

#endif
