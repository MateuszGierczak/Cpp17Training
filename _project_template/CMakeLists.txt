if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

message(STATUS "Vcpkg integration script found : " ${CMAKE_TOOLCHAIN_FILE})

get_filename_component(PROJECT_NAME_STR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

cmake_minimum_required(VERSION 3.8)

project(${PROJECT_NAME_STR})

find_package(Boost REQUIRED)

file(GLOB HEADER_FILES_LIST "*.h" "*.hpp")
file(GLOB SOURCE_FILES_LIST "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES_LIST}
                               ${HEADER_FILES_LIST})
target_link_libraries(${PROJECT_NAME} Boost::boost)

enable_testing()
add_test(tests ${PROJECT_NAME})
