set(MODULE_NAME utils)

add_library(${MODULE_NAME} INTERFACE)
add_library(libs::${MODULE_NAME} ALIAS ${MODULE_NAME})
target_include_directories(${MODULE_NAME} INTERFACE include/)
target_link_libraries(${MODULE_NAME} 
    INTERFACE
        libs::traits)

find_package(GTest REQUIRED)

add_executable(
    ${MODULE_NAME}_ut
    ut/RangePrinterTests.cpp
)

target_link_libraries(${MODULE_NAME}_ut
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
        libs::utils
)

add_test(utils_gtests ${MODULE_NAME}_ut)