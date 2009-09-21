#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (VECTOR_SRCS
vtkGRASSVectorFeaturePoints.cxx
vtkGRASSVectorFeatureCats.cxx
vtkGRASSVectorMapBase.cxx
vtkGRASSVectorMapTopoReader.cxx
vtkGRASSVectorMapNoTopoReader.cxx
vtkGRASSVectorBBox.cxx
)

SET (VECTOR_H
vtkGRASSVectorFeaturePoints.h
vtkGRASSVectorFeatureCats.h
vtkGRASSVectorMapBase.h
vtkGRASSVectorMapTopoReader.h
vtkGRASSVectorMapNoTopoReader.h
vtkGRASSVectorBBox.h
vtkGRASSBridgeVectorWin32Header.h
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeVector C++ library.
ADD_LIBRARY (vtkGRASSBridgeVector ${VECTOR_SRCS}  ${VECTOR_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeVector vtkGRASSBridgeCommon vtkCommon ${GRASS_gis_LIBRARY} ${GRASS_vect_LIBRARY})

# Create the vtkGRASSBridgeVector Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeVectorJava VECTORJava_SRCS "${VECTOR_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeVectorJava SHARED ${VECTORJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeVectorJava vtkGRASSBridgeVector vtkGRASSBridgeCommonJava vtkCommonJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeLibraryFilter Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeVectorPython VECTORPython_SRCS "${VECTOR_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeVectorPythonD ${VECTORPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeVectorPython MODULE vtkGRASSBridgeVectorPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeVectorPythonD vtkGRASSBridgeVector vtkGRASSBridgeCommonPythonD )
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeVectorPython vtkGRASSBridgeVectorPythonD)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeVector vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeVector vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeVector vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeVector vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${VECTOR_SRCS} ${VECTOR_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeVector)

