get_filename_component(PROJECT_NAME_STR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

cmake_minimum_required(VERSION 3.8)

project(${PROJECT_NAME_STR})

file(GLOB HEADER_FILES_LIST "*.h" "*.hpp")
file(GLOB SOURCE_FILES_LIST "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES_LIST}
                               ${HEADER_FILES_LIST})

enable_testing()
add_test(tests ${PROJECT_NAME})
