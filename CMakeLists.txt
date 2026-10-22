cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(BCPetSim)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(bcpet BCPet.cc ${sources})
target_link_libraries(bcpet ${Geant4_LIBRARIES})
set_target_properties(bcpet PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)