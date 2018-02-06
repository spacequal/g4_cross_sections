#CXX Standard
set (CMAKE_CXX_STANDARD 14)

#Compile all source files as executables in this directory
file(GLOB srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

#ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
foreach(lib_list ${ROOT_LIBRARIES})
	message(STATUS ${lib_list})
endforeach()


#GEANT4 Package Search
list(APPEND CMAKE_PREFIX_PATH $ENV{G4INSTALL})
option(WITH_GEANT4_UIVIS "UI and VIS Drivers" OFF)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

#Main Project Include
include_directories(${PROJECT_SOURCE_DIR}/include)

foreach(src ${srcs})
	string(REPLACE ".cc" "" ename ${src})
	add_executable(${ename} ${src})
#	target_link_libraries(${ename} cs_draw)
	target_link_libraries(${ename} ${ROOT_LIBRARIES})
	target_link_libraries(${ename} ${Geant4_LIBRARIES})
endforeach()
