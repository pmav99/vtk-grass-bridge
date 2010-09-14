#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (DBMI_SRCS
vtkGRASSDbmiInterface.cxx
vtkGRASSDbmiTable.cxx
vtkGRASSDbmiColumn.cxx
)

SET (DBMI_H
vtkGRASSDbmiInterface.h
vtkGRASSDbmiTable.h
vtkGRASSDbmiColumn.h
vtkGRASSBridgeDbmiWin32Header.h
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeDbmi C++ library.
ADD_LIBRARY (vtkGRASSBridgeDbmi ${DBMI_SRCS}  ${DBMI_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeDbmi vtkGRASSBridgeCommon vtkCommon  ${GRASS_LIBRARIES})

# Create the vtkGRASSBridgeDbmi Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeDbmiJava DBMIJava_SRCS "${DBMI_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeDbmiJava SHARED ${DBMIJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeDbmiJava vtkGRASSBridgeDbmi vtkGRASSBridgeCommonJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeDbmiJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeLibraryFilter Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeDbmiPython DBMIPython_SRCS "${DBMI_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeDbmiPythonD ${DBMIPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeDbmiPython MODULE vtkGRASSBridgeDbmiPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeDbmiPythonD vtkGRASSBridgeDbmi vtkGRASSBridgeCommonPythonD )
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeDbmiPython vtkGRASSBridgeDbmiPythonD)

  INSTALL_TARGETS(/lib vtkGRASSBridgeDbmiPythonD vtkGRASSBridgeDbmiPython)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeDbmi vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeDbmi vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeDbmi vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeDbmi vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${DBMI_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeDbmi)

