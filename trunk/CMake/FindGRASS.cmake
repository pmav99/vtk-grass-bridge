#
# Find the native GRASS includes and library
#
# This module defines
# GRASS_INCLUDE_DIR, where to find avcodec.h, avformat.h ...
# GRASS_LIBRARIES, the libraries to link against to use GRASS.
# GRASS_FOUND, If false, do not try to use GRASS.

FIND_PATH(GRASS_INCLUDE_DIR grass/gis.h
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/include
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(GRASS_gis_LIBRARY grass_gis
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_raster_LIBRARY grass_raster
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_g3d_LIBRARY grass_g3d
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_vector_LIBRARY grass_vector
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_gmath_LIBRARY grass_gmath
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_gpde_LIBRARY grass_gpde
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_dbmibase_LIBRARY grass_dbmibase
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_dbmidriver_LIBRARY grass_dbmidriver
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_dbmiclient_LIBRARY grass_dbmiclient
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_datetime_LIBRARY grass_datetime
  /1/gebbert/src/grass_trunk/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

IF(GRASS_INCLUDE_DIR)
  IF(GRASS_gis_LIBRARY)
    IF(GRASS_g3d_LIBRARY)
      IF(GRASS_vect_LIBRARY)
        SET( GRASS_FOUND "YES" )
        SET( GRASS_BASIC_LIBRARIES 
          ${GRASS_gis_LIBRARY} 
          ${GRASS_datetime_LIBRARY} 
          ${GRASS_g3d_LIBRARY}
          ${GRASS_vect_LIBRARY} 
          )
        SET( GRASS_LIBRARIES 
          ${GRASS_BASIC_LIBRARIES}
          ${GRASS_dbmibase_LIBRARY}
          ${GRASS_raster_LIBRARY}
          ${GRASS_dbmiclient_LIBRARY}
          ${GRASS_dbmidriver_LIBRARY}
          ${GRASS_gmath_LIBRARY}
          ${GRASS_gpde_LIBRARY})
      ENDIF(GRASS_vect_LIBRARY)
    ENDIF(GRASS_g3d_LIBRARY)
  ENDIF(GRASS_gis_LIBRARY)
ENDIF(GRASS_INCLUDE_DIR)

MARK_AS_ADVANCED(
  GRASS_INCLUDE_DIR
  GRASS_gis_LIBRARY 
  GRASS_datetime_LIBRARY 
  GRASS_g3d_LIBRARY 
  GRASS_raster_LIBRARY 
  GRASS_vect_LIBRARY 
  GRASS_gmath_LIBRARY 
  GRASS_gpde_LIBRARY 
  GRASS_dbmiclient_LIBRARY 
  GRASS_dbmidriver_LIBRARY 
  GRASS_dbmibase_LIBRARY 
  )