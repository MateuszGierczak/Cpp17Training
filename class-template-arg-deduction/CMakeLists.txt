get_filename_component(PROJECT_NAME_STR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME_STR})

find_package(Catch2 CONFIG REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} tests.cpp)

target_link_libraries(${PROJECT_NAME} Boost::headers Catch2::Catch2 Catch2::Catch2WithMain)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

enable_testing()
add_test(tests ${PROJECT_NAME})
