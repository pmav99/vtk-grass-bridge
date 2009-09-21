#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (Filtering_SRCS
)

SET (Filtering_H
)
# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeFiltering C++ library.
ADD_LIBRARY (vtkGRASSBridgeFiltering ${Filtering_SRCS} ${Filtering_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeFiltering vtkGRASSBridgeCommon vtkCommon vtkFiltering)
#TARGET_LINK_LIBRARIES(vtkGRASSBridgeFiltering)


# Create the vtkGRASSBridgeFiltering Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeFilteringJava FilteringJava_SRCS "${Filtering_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeFilteringJava SHARED ${FilteringJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeFilteringJava vtkGRASSBridgeFiltering vtkFilteringJava vtkCommonJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeFiltering Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeFilteringPython FilteringPython_SRCS "${Filtering_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeFilteringPythonD ${FilteringPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeFilteringPython MODULE vtkGRASSBridgeFilteringPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeFilteringPythonD vtkGRASSBridgeFiltering vtkFilteringPythonD vtkCommonPythonD)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeFilteringPython vtkGRASSBridgeFilteringPythonD)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeFiltering vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeFiltering vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeFiltering vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeFiltering vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${Filtering_SRCS}  ${Filtering_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeFiltering)

