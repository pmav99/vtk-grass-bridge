#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (Common_SRCS
vtkGRASSDatabaseInfo.cxx
vtkGRASSFileLister.cxx
vtkGRASSRasterMapLister.cxx
vtkGRASSRaster3dMapLister.cxx
vtkGRASSVectorMapLister.cxx
vtkGRASSRegion.cxx
vtkObjectGRASSErrorHandler.cxx
vtkGRASSInit.cxx
vtkGRASSModule.cxx
vtkGRASSFlag.cxx
vtkGRASSOption.cxx
vtkGRASSOptionFactory.cxx
vtkGRASSMessagingInterface.cxx
)

SET (Common_H
vtkGRASSDatabaseInfo.h
vtkGRASSFileLister.h
vtkGRASSRasterMapLister.h
vtkGRASSRaster3dMapLister.h
vtkGRASSVectorMapLister.h
vtkGRASSRegion.h
vtkGRASSInit.h
vtkGRASSModule.h
vtkGRASSFlag.h
vtkGRASSOption.h
vtkGRASSOptionFactory.h
vtkObjectGRASSErrorHandler.h
vtkGRASSMessagingInterface.h
vtkGRASSBridgeCommonWin32Header.h
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeCommon C++ library.
ADD_LIBRARY (vtkGRASSBridgeCommon ${Common_SRCS}  ${Common_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeCommon vtkCommon ${GRASS_LIBRARIES})

# Create the vtkGRASSBridgeCommon Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeCommonJava CommonJava_SRCS "${Common_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeCommonJava SHARED ${CommonJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeCommonJava vtkCommonJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeCommonJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeLibraryFilter Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeCommonPython CommonPython_SRCS "${Common_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeCommonPythonD ${CommonPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeCommonPython MODULE vtkGRASSBridgeCommonPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeCommonPythonD vtkGRASSBridgeCommon vtkCommonPythonD )
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeCommonPython vtkGRASSBridgeCommonPythonD)

  INSTALL_TARGETS(/lib vtkGRASSBridgeCommonPythonD vtkGRASSBridgeCommonPython)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeCommon vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeCommon vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeCommon vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeCommon vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${Common_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeCommon)
