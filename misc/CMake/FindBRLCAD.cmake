# - Find BRLCAD 
#
# This module defines the following variables:
#  BRLCAD_FOUND
#  BRLCAD_INCLUDE_DIR
#  BRLCAD_INCLUDE_DIRS
#  BRLCAD_BU_LIBRARY
#  BRLCAD_BN_LIBRARY
#  BRLCAD_RT_LIBRARY
#  BRLCAD_WDB_LIBRARY
#


find_path(BRLCAD_INCLUDE_DIR brlcad/bu.h 
  DOC "Include directory for BRLCAD" NO_DEFAULT_PATH 
  PATHS "${CMAKE_INSTALL_PREFIX}/include"
        "${BRLCAD_ROOT}/stable/include"
        "${BRLCAD_ROOT}/include"
        "$ENV{BRLCAD_ROOT}/stable/include"
        "$ENV{BRLCAD_ROOT}/include"
	"/usr/brlcad/stable/include"
	"/usr/brlcad/include"
	)
# MESSAGE( STATUS "brlcad_include ${BRLCAD_INCLUDE_DIR}" )
if (BRLCAD_INCLUDE_DIR)
  # MESSAGE( STATUS "setting brlcad dirs ${BRLCAD_INCLUDE_DIR}" )
  set(BRLCAD_INCLUDE_DIRS 
    ${BRLCAD_INCLUDE_DIR}
    ${BRLCAD_INCLUDE_DIR}/brlcad
    ${BRLCAD_INCLUDE_DIR}/openNURBS 
    CACHE PATH "List of BRLCAD install directories" )
  # MESSAGE( STATUS "brlcad_include_dirs ${BRLCAD_INCLUDE_DIRS}" )
endif(BRLCAD_INCLUDE_DIR)

SET(BRLCAD_LIB_SEARCH_DIRS
  "${CMAKE_INSTALL_PREFIX}/lib"
  "${BRLCAD_ROOT}/stable/lib"
  "${BRLCAD_ROOT}/lib"
  "$ENV{BRLCAD_ROOT}/stable/lib"
  "$ENV{BRLCAD_ROOT}/lib"
  "/usr/brlcad/stable/lib"
  "/usr/brlcad/lib")
  
find_library(BRLCAD_BU_LIBRARY bu NO_DEFAULT_PATH 
  PATHS ${BRLCAD_LIB_SEARCH_DIRS} )

find_library(BRLCAD_BN_LIBRARY bn NO_DEFAULT_PATH 
  PATHS ${BRLCAD_LIB_SEARCH_DIRS} )

find_library(BRLCAD_WDB_LIBRARY wdb NO_DEFAULT_PATH 
  PATHS ${BRLCAD_LIB_SEARCH_DIRS} )

if(BRLCAD_BU_LIBRARY) 
  # if you did not find libbu, then the rt library you find is not 
  # going to be the right one (linux has a deprecated librt for realtime 
  # exensions which is mostly subsumed into libc these days
  find_library(BRLCAD_RT_LIBRARY rt NO_DEFAULT_PATH
    PATHS ${BRLCAD_LIB_SEARCH_DIRS} )
endif(BRLCAD_BU_LIBRARY) 

get_filename_component(BRLCAD_LIBRARY_DIR "${BRLCAD_BU_LIBRARY}" PATH)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BRLCAD DEFAULT_MSG BRLCAD_LIBRARY_DIR BRLCAD_BU_LIBRARY BRLCAD_BN_LIBRARY BRLCAD_RT_LIBRARY BRLCAD_WDB_LIBRARY BRLCAD_INCLUDE_DIR)

