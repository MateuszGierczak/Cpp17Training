set(MODULE_NAME traits)

add_library(${MODULE_NAME} INTERFACE)
add_library(libs::${MODULE_NAME} ALIAS ${MODULE_NAME})
target_include_directories(${MODULE_NAME} INTERFACE include/)