#CXX Standard
set(CMAKE_CXX_STANDARD 14)

#Compile all source files as executables in this directory
file(GLOB srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

#ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

#GEANT4 Package Search
list(APPEND CMAKE_PREFIX_PATH $ENV{G4INSTALL})
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

#Unit Test Framework
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(Check REQUIRED)
include("${PROJECT_SOURCE_DIR}/cmake/FindCheck.cmake")


#GSL and Threads
find_package(GSL REQUIRED)
find_package(Threads REQUIRED)

#Main Project Include
set(libs ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${GSL_LIBRARIES} ${CHECK_LIBRARIES})
set(incs ${GSL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
include_directories(${incs})

foreach(src ${srcs})
	string(REPLACE ".cc" "" ename ${src})
	add_executable(${ename} ${src})
	target_link_libraries(${ename} Threads::Threads ${libs} Threads::Threads)
endforeach()

