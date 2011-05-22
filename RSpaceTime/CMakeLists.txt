#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (RSpaceTime_SRCS
  vtkRInterfaceSpatial.cxx
  vtkRInterfaceSpaceTime.cxx
)

SET (RSpaceTime_H
  vtkGRASSBridgeRSpaceTimeWin32Header.h
  vtkRInterfaceSpatial.h
  vtkRInterfaceSpaceTime.h
)
# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeRSpaceTime C++ library.
ADD_LIBRARY (vtkGRASSBridgeRSpaceTime ${RSpaceTime_SRCS} ${RSpaceTime_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeRSpaceTime vtkGRASSBridgeCommon vtkFiltering vtkGraphics)

# Create the vtkGRASSBridgeRSpaceTime Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeRSpaceTimeJava RSpaceTimeJava_SRCS "${RSpaceTime_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeRSpaceTimeJava SHARED ${RSpaceTimeJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRSpaceTimeJava vtkGRASSBridgeRSpaceTime vtkGraphicsJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeRSpaceTimeJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeRSpaceTime Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeRSpaceTimePython RSpaceTimePython_SRCS "${RSpaceTime_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeRSpaceTimePythonD ${RSpaceTimePython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeRSpaceTimePython MODULE vtkGRASSBridgeRSpaceTimePythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeRSpaceTimePythonD vtkGRASSBridgeRSpaceTime vtkGraphicsPythonD vtkCommonPythonD)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeRSpaceTimePython vtkGRASSBridgeRSpaceTimePythonD)


  INSTALL_TARGETS(/lib vtkGRASSBridgeRSpaceTimePython vtkGRASSBridgeRSpaceTimePythonD)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRSpaceTime vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRSpaceTime vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRSpaceTime vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRSpaceTime vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${RSpaceTime_SRCS} ${RSpaceTime_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeRSpaceTime)

