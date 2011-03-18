#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (Graphics_SRCS
  vtkRInterfaceSpatial.cxx
  vtkRInterfaceSpaceTime.cxx
)

SET (Graphics_H
  vtkGRASSBridgeGraphicsWin32Header.h
  vtkRInterfaceSpatial.h
  vtkRInterfaceSpaceTime.h
)
# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeGraphics C++ library.
ADD_LIBRARY (vtkGRASSBridgeGraphics ${Graphics_SRCS} ${Graphics_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeGraphics vtkGRASSBridgeCommon vtkFiltering vtkGraphics)

# Create the vtkGRASSBridgeGraphics Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeGraphicsJava GraphicsJava_SRCS "${Graphics_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeGraphicsJava SHARED ${GraphicsJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeGraphicsJava vtkGRASSBridgeGraphics vtkGraphicsJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeGraphicsJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeGraphics Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeGraphicsPython GraphicsPython_SRCS "${Graphics_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeGraphicsPythonD ${GraphicsPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeGraphicsPython MODULE vtkGRASSBridgeGraphicsPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeGraphicsPythonD vtkGRASSBridgeGraphics vtkGraphicsPythonD vtkCommonPythonD)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeGraphicsPython vtkGRASSBridgeGraphicsPythonD)


  INSTALL_TARGETS(/lib vtkGRASSBridgeGraphicsPython vtkGRASSBridgeGraphicsPythonD)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

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

