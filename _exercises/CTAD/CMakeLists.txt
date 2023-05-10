get_filename_component(PROJECT_NAME_STR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME_STR}_exc)

find_package(Catch2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} tests.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2 
                                              Catch2::Catch2WithMain
                                              utils)

enable_testing()
add_test(tests ${PROJECT_NAME})
