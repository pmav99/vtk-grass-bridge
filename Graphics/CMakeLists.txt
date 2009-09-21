#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (Graphics_SRCS
)

SET (Graphics_H
vtkGRASSBridgeGraphicsWin32Header.h
)
# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeGraphics C++ library.
ADD_LIBRARY (vtkGRASSBridgeGraphics ${Graphics_SRCS} ${Graphics_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeGraphics vtkGRASSBridgeCommon vtkFiltering vtkGraphics)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeGraphics vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeGraphics vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeGraphics vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeGraphics vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${Graphics_SRCS} ${Graphics_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeGraphics)

