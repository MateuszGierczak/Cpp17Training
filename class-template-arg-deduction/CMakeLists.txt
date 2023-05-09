if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

message(STATUS "Vcpkg integration script found : " ${CMAKE_TOOLCHAIN_FILE})

get_filename_component(PROJECT_NAME_STR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

cmake_minimum_required(VERSION 3.13)

project(${PROJECT_NAME_STR})

find_package(Catch2 CONFIG REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} tests.cpp)

target_link_libraries(${PROJECT_NAME} Boost::headers Catch2::Catch2 Catch2::Catch2WithMain)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

enable_testing()
add_test(tests ${PROJECT_NAME})
