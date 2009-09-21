/*=========================================================================
 VTK GRASS Bridge
 Authors: Soeren Gebbert
 Created: Aug. 2007


 =========================================================================*/
/// \brief manage Windows system differences
/// 
/// The Header captures some system differences between Unix
/// and Windows operating systems. 

#ifndef __vtkGRASSBridgeGraphicsWin32Header_h
#define __vtkGRASSBridgeGraphicsWin32Header_h

#include <vtkGRASSBridgeConfigure.h>

#if defined(WIN32) && !defined(VTK_GRASS_BRIDGE_STATIC)
#if defined(vtkGRASSBridgeGraphics_EXPORTS)
#define VTK_GRASS_BRIDGE_GRAPHICS_EXPORT __declspec( dllexport ) 
#else
#define VTK_GRASS_BRIDGE_GRAPHICS_EXPORT __declspec( dllimport ) 
#endif
#else
#define VTK_GRASS_BRIDGE_GRAPHICS_EXPORT
#endif

#endif
