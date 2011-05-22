#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (Temporal_SRCS
vtkTemporalDataSetSource.cxx
)

SET (Temporal_H
vtkGRASSBridgeTemporalWin32Header.h
vtkTemporalDataSetSource.h
)
# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeTemporal C++ library.
ADD_LIBRARY (vtkGRASSBridgeTemporal ${Temporal_SRCS} ${Temporal_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeTemporal vtkGRASSBridgeCommon vtkCommon vtkFiltering)
#TARGET_LINK_LIBRARIES(vtkGRASSBridgeTemporal)


# Create the vtkGRASSBridgeTemporal Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeTemporalJava TemporalJava_SRCS "${Temporal_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeTemporalJava SHARED ${TemporalJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeTemporalJava vtkGRASSBridgeTemporal vtkFilteringJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeTemporalJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeTemporal Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeTemporalPython TemporalPython_SRCS "${Temporal_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeTemporalPythonD ${TemporalPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeTemporalPython MODULE vtkGRASSBridgeTemporalPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeTemporalPythonD vtkGRASSBridgeTemporal vtkFilteringPythonD vtkCommonPythonD)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeTemporalPython vtkGRASSBridgeTemporalPythonD)


  INSTALL_TARGETS(/lib vtkGRASSBridgeTemporalPython vtkGRASSBridgeTemporalPythonD)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeTemporal vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeTemporal vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeTemporal vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeTemporal vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${Temporal_SRCS}  ${Temporal_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeTemporal)

