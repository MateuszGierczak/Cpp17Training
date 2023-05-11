if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

message(STATUS "Vcpkg integration script found : " ${CMAKE_TOOLCHAIN_FILE})

get_filename_component(PROJECT_NAME_STR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

cmake_minimum_required(VERSION 3.13)

project(${PROJECT_NAME_STR} CXX)

add_subdirectory(_project_template)
add_subdirectory(examples)
add_subdirectory(exercises)
add_subdirectory(utils)