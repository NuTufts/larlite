cmake_minimum_required(VERSION 3.10)
project(larlite VERSION 1.1.0)

#---Install location by default is in the build directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "${CMAKE_BUILD_DIR}" FORCE )
endif()

#---Module directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

## Dependencies

#---CERN ROOT
find_package(ROOT CONFIG REQUIRED)

#---PYTHON
if (USE_PYTHON3)
  message(STATUS "Using Python 3")
  find_package( PythonInterp 3 REQUIRED )
  find_package( PythonLibs 3 REQUIRED )
  # warning gross hack
  execute_process( COMMAND python3 -c "import numpy; print(numpy.get_include())" OUTPUT_VARIABLE NUMPY_INCLUDES OUTPUT_STRIP_TRAILING_WHITESPACE )
  #message(${NUMPY_INCLUDES})
  set(LARLITE_USE_PYTHON 1)
  set(LARLITE_PYTHON_VERSION 3)
elseif(USE_PYTHON2)
  message(STATUS "Using Python 2")  
  find_package( PythonInterp 2.7 REQUIRED )
  find_package( PythonLibs 2.7 REQUIRED )
  set(LARLITE_USE_PYTHON 1)
  set(LARLITE_PYTHON_VERSION 2)
else()
  set(LARLITE_USE_PYTHON 0)
endif()

## Git Submodules

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  # Update submodules as needed
  option(GIT_SUBMODULE "Check submodules during build" ON)
  if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
  endif()
endif()

## Setup

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

#---Define Variables for subdirectory cmakelists
set(LARLITE_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

## Add Source

#---Add base of source
add_subdirectory(larlite)

#---Configuration export
include(CMakePackageConfigHelpers)
set(LARLITE_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LARLITE_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(TARGETS_EXPORT_NAME "larliteTargets")
configure_package_config_file("cmake/Config.cmake.in"
  "${CMAKE_BINARY_DIR}/cmake/larliteConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/larlite"
  PATH_VARS LARLITE_INCLUDE_DIR LARLITE_LIBRARY_DIR LARLITE_BASE_DIR LARLITE_USE_PYTHON LARLITE_PYTHON_VERSION)
write_basic_package_version_file("${CMAKE_BINARY_DIR}/cmake/larliteConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion)


## Export Targets

#---Create info to include larlite in other cmake files
install(EXPORT larlite
        FILE larliteTargets.cmake
        DESTINATION "${CMAKE_BINARY_DIR}/installed/lib/cmake/larlite")

#---Create info to include larlite in other cmake files
install(FILES "${CMAKE_BINARY_DIR}/cmake/larliteConfig.cmake"
  "${CMAKE_BINARY_DIR}/cmake/larliteConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/larlite")
