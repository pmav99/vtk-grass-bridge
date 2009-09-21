#
# Find the native GRASS includes and library
#
# This module defines
# GRASS_INCLUDE_DIR, where to find avcodec.h, avformat.h ...
# GRASS_LIBRARIES, the libraries to link against to use GRASS.
# GRASS_FOUND, If false, do not try to use GRASS.

FIND_PATH(GRASS_INCLUDE_DIR grass/gis.h
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/include
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(GRASS_gis_LIBRARY grass_gis
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_raster_LIBRARY grass_raster
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_g3d_LIBRARY grass_g3d
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_vect_LIBRARY grass_vect
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_gmath_LIBRARY grass_gmath
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_gpde_LIBRARY grass_gpde
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_dbmibase_LIBRARY grass_dbmibase
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_dbmidriver_LIBRARY grass_dbmidriver
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_dbmiclient_LIBRARY grass_dbmiclient
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

FIND_LIBRARY(GRASS_datetime_LIBRARY grass_datetime
  /home/soeren/src/grass6.4dev/grass-6.4.0RC5/dist.i686-pc-linux-gnu/lib
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
