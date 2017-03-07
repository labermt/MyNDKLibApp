# file: MyNDKLibApp/MyNDKModule/src/main/cpp/MyCppLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

add_library(MyCppLib SHARED src/MyCppClass.cpp)

# copy out the lib binary and remove generated files
set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../distribution)
set_target_properties(MyCppLib
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY
                      "${distribution_DIR}/MyCppLib/lib/${ANDROID_ABI}")
add_custom_command(TARGET MyCppLib POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy "${CMAKE_CURRENT_SOURCE_DIR}/src/MyCppClass.h"
                   "${distribution_DIR}/MyCppLib/include/MyCppClass.h"
                   COMMENT "Copying MyCppLib to output directory")
