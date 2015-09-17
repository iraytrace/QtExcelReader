project( excelReader )
cmake_minimum_required(VERSION 3.2)

# Allow all sub CMakeLists to use our custom distributed CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/misc/CMake"
  "${CMAKE_SOURCE_DIR}/misc/CMake/compiler"
  "${CMAKE_SOURCE_DIR}/misc/nsis"
)

include(setBuildMachineUser) #create definitions for whence the executable is built
#include(compiler_setup)
include(findAndInstallDependencies)
include(InstallRequiredSystemLibraries)
include(setupCpack)
setupCpack(excelReader 1 0 "${CMAKE_SOURCE_DIR}/misc/license.txt")

# target build locations
set(RUNTIME_DIR bin)
set(PLUGIN_DIR ${RUNTIME_DIR}/plugins)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/lib)


find_package(OpenGL REQUIRED)


# Get the Qt stuff set up.
# if cmake cannot find Qt you can set Qt5_DIR or uncomment next line
set(Qt5_DIR "C:/Qt/5.5/msvc2013_64/lib/cmake/Qt5")
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)

# make all sources part of the build
file(GLOB product_SRCS
  "${CMAKE_CURRENT_LIST_DIR}/*cpp"
  "${CMAKE_CURRENT_LIST_DIR}/*h"
  "${CMAKE_CURRENT_LIST_DIR}/*qrc")

file(GLOB product_UIS "${CMAKE_CURRENT_LIST_DIR}/*ui") # Make all *.ui files part of the build

include_directories(${PROJECT_BINARY_DIR}) # necessary to pick up uic headers
include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5OpenGL_INCLUDE_DIRS}
    )

qt5_wrap_ui(UIS ${product_UIS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (WIN32)
    add_executable(cool WIN32 ${product_SRCS} ${UIS} )
else (WIN32)
    add_executable(cool       ${product_SRCS} ${UIS} )
endif (WIN32)

target_link_LIBRARIES(cool
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
  )

set_target_properties(cool
    PROPERTIES
    ENABLE_EXPORTS ON
    COMPILE_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

install(TARGETS cool RUNTIME DESTINATION ${RUNTIME_DIR})
find_and_install_dependencies(cool)


