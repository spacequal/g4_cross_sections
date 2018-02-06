#Cross-Section Data Extraction and Plotting Tools from GEANT
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(cs_draw)
add_subdirectory(bin)
set (CMAKE_CXX_STANDARD 14)

#ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT COMPONENTS Core Hist Graf)
include(${ROOT_USE_FILE})

#GEANT4 Package Search
list(APPEND CMAKE_PREFIX_PATH $ENV{G4INSTALL})
option(WITH_GEANT4_UIVIS "UI and VIS Drivers" OFF)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

#Tools
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#add_library(cs_draw ${src})
#target_link_libraries(cs_draw ${ROOT_LIBRARIES})
#file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
#target_include_directories(cs_draw PUBLIC include)

