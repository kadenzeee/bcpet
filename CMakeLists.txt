cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(BCPetSim)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

find_program(ROOT_CONFIG root-config)
if(NOT ROOT_CONFIG)
    message(FATAL_ERROR "root-config not found")
endif()

execute_process(
    COMMAND ${ROOT_CONFIG} --incdir
    OUTPUT_VARIABLE ROOT_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${ROOT_CONFIG} --libs
    OUTPUT_VARIABLE ROOT_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(bcpet BCPet.cc ${sources})
target_link_libraries(bcpet PRIVATE ${ROOT_LIBRARIES} ${Geant4_LIBRARIES})
set_target_properties(bcpet PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
