#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (IO_SRCS
vtkGRASSRasterImageReader.cxx
vtkGRASSRaster3dImageReader.cxx
vtkGRASSRasterImageWriter.cxx
vtkGRASSRaster3dImageWriter.cxx
vtkGRASSRasterToImageReader.cxx
vtkGRASSVectorPolyDataReader.cxx
vtkGRASSVectorPolyDataWriter.cxx
vtkGRASSVectorPolyDataAreaWriter.cxx
vtkGRASSVectorTopoPolyDataReader.cxx
vtkGRASSMultiVectorPolyDataLineReader.cxx
vtkGRASSMultiRasterPolyDataLineReader.cxx
)

SET (IO_H
vtkGRASSRasterImageReader.h
vtkGRASSRaster3dImageReader.h
vtkGRASSRasterImageWriter.h
vtkGRASSRaster3dImageWriter.h
vtkGRASSRasterToImageReader.h
vtkGRASSVectorPolyDataReader.h
vtkGRASSVectorPolyDataWriter.h
vtkGRASSVectorPolyDataAreaWriter.h
vtkGRASSVectorTopoPolyDataReader.h
vtkGRASSBridgeIOWin32Header.h
vtkGRASSMultiVectorPolyDataLineReader.h
vtkGRASSMultiRasterPolyDataLineReader.h
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeIO C++ library.
ADD_LIBRARY (vtkGRASSBridgeIO ${IO_SRCS}  ${IO_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeIO vtkGRASSBridgeCommon vtkGRASSBridgeRaster vtkGRASSBridgeRaster3d vtkGRASSBridgeVector vtkIO vtkFiltering vtkCommon vtkImaging  ${GRASS_LIBRARIES})

# Create the vtkGRASSBridgeIO Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeIOJava IOJava_SRCS "${IO_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeIOJava SHARED ${IOJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeIOJava vtkGRASSBridgeIO vtkGRASSBridgeCommonJava vtkGRASSBridgeRasterJava vtkGRASSBridgeRaster3dJava vtkGRASSBridgeVectorJava vtkIOJava vtkFilteringJava vtkImagingJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeIOJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeLibraryFilter Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeIOPython IOPython_SRCS "${IO_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeIOPythonD ${IOPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeIOPython MODULE vtkGRASSBridgeIOPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeIOPythonD vtkGRASSBridgeIO vtkGRASSBridgeCommonPythonD vtkGRASSBridgeRasterPythonD vtkGRASSBridgeRaster3dPythonD vtkGRASSBridgeVectorPythonD vtkIOPythonD vtkFilteringPythonD vtkImagingPythonD)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeIOPython vtkGRASSBridgeIOPythonD)

  INSTALL_TARGETS(/lib vtkGRASSBridgeIOPythonD vtkGRASSBridgeIOPython)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeIO vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeIO vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeIO vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeIO vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${IO_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeIO)

