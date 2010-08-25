#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (IO_SRCS
vtkGRASSRasterMapBase.cxx
vtkGRASSRasterMapReader.cxx
vtkGRASSRasterMapWriter.cxx
vtkGRASSHistory.cxx
)

SET (IO_H
vtkGRASSRasterMapBase.h
vtkGRASSRasterMapReader.h
vtkGRASSRasterMapWriter.h
vtkGRASSHistory.h
vtkGRASSBridgeRasterWin32Header.h
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeRaster C++ library.
ADD_LIBRARY (vtkGRASSBridgeRaster ${IO_SRCS}  ${IO_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeRaster vtkGRASSBridgeCommon vtkCommon ${GRASS_gis_LIBRARY} ${GRASS_g3d_LIBRARY} ${GRASS_raster_LIBRARY})

# Create the vtkGRASSBridgeRaster Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeRasterJava IOJava_SRCS "${IO_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeRasterJava SHARED ${IOJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRasterJava vtkGRASSBridgeRaster vtkGRASSBridgeCommonJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeRasterJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeLibraryFilter Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeRasterPython IOPython_SRCS "${IO_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeRasterPythonD ${IOPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeRasterPython MODULE vtkGRASSBridgeRasterPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeRasterPythonD vtkGRASSBridgeRaster vtkGRASSBridgeCommonPythonD)
INSTALL_TARGETS(/lib vtkGRASSBridgeRasterPythonD vtkGRASSBridgeRasterPython)

  TARGET_LINK_LIBRARIES(vtkGRASSBridgeRasterPython vtkGRASSBridgeRasterPythonD)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${IO_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeRaster)

