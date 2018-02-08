#Cross-Section Data Extraction and Plotting Tools from GEANT
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(g4_cross_sections)
add_subdirectory(bin)
add_subdirectory(test)

#Threads
find_package(Threads REQUIRED)

#Compiler Standard
set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#ROOT
set(CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED Core Hist Graf)
include(${ROOT_USE_FILE})

#GEANT4 Package Search
set(CMAKE_PREFIX_PATH $ENV{G4INSTALL})
option(WITH_GEANT4_UIVIS "UI and VIS Drivers" OFF)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

#Tools
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#add_library(g4_cs ${src})
#target_link_libraries(g4_cs ${ROOT_LIBRARIES})
#file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
#target_include_directories(g4_cs PUBLIC include)

#Enable Unit Testing
enable_testing()
add_test(NAME gsl_check     COMMAND gsl)
#add_test(NAME g4_fail      COMMAND gsl)
#add_test(NAME g4_mega_fail COMMAND gsl)

