#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(sdf2root)

# I wanna compile with C++11
add_definitions("-std=c++11 -march=native -mtune=native")

include(FindROOT.cmake)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)

#----------------------------------------------------------------------------
# set everything needed for the root dictonary and create the
# dictionary
#
set(SDF2ROOT_SRCS
  src/TMeshValue.cpp
)

# fill list of header files 
set(SDF2ROOT_HEADERS
  include/TMeshValue.hpp
  include/TMacroParticle.hpp
)

set(SDF2ROOT_LINKDEF include/SDF2ROOTLinkDef.hpp)
set(SDF2ROOT_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/SDF2ROOTDict.cpp) 
ROOT_GENERATE_DICTIONARY("${SDF2ROOT_HEADERS}" "${SDF2ROOT_LINKDEF}"
                         "${SDF2ROOT_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

# add the dictionary to the list of source files
set(SDF2ROOT_SRCS ${SDF2ROOT_SRCS} ${SDF2ROOT_DICTIONARY}) 

#----------------------------------------------------------------------------
# Generating library
#
add_library(SDF2ROOT SHARED ${SDF2ROOT_SRCS})
target_link_libraries(SDF2ROOT ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Generating executable file and linking
#
add_executable(sdf2root main.cpp ${sources} ${headers})
target_link_libraries(sdf2root ${ROOT_LIBRARIES} SDF2ROOT)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS sdf2root DESTINATION bin)

