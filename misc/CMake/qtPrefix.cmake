# make a reasonable attempt at finding where Qt lives on Windows
if (WIN32)
  # Define QTPREFIX to find Qt installations in a non-standard location
  if(NOT DEFINED QTPREFIX)
    set(QTPREFIX "C:/Qt")
  endif(NOT DEFINED QTPREFIX)
  file(GLOB QTPATH "${QTPREFIX}/${QTVERSION_DIR}/bin/qmake.exe")
  string(REPLACE "//" "/" QTPATH "${QTPATH}")
  string(REPLACE "/bin/qmake.exe" "" QTPATH "${QTPATH}")
  set(CMAKE_PREFIX_PATH "${QTPATH}")
  message(STATUS "QTPATH=${QTPATH}")
endif (WIN32)


