###############################################################################
# Compiler configuration
###############################################################################
# This approach to managing compiler settings was adopted from interactions
# between Jeff Amstutz and Intel Corporation.

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  include(gcc)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
  include(icc)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  include(clang)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
message(STATUS ">${CMAKE_CXX_COMPILER_ID}<")
  include(msvc)
  if(NOT DEFINED QTVERSION_DIR)
    set(QTVERSION_DIR "5.4/msvc2013_64_opengl")
  endif(NOT DEFINED QTVERSION_DIR)
else()
  message(FATAL_ERROR "Unknown compiler specified: < ${CMAKE_CXX_COMPILER_ID} >")
endif()

