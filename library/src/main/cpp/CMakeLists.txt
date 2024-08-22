# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

#cmake_minimum_required(VERSION 3.10.2)
#
## Declares and names the project.
#
#project("tinyalas4android")
#
## Creates and names a library, sets it as either STATIC
## or SHARED, and provides the relative paths to its source code.
## You can define multiple libraries, and CMake builds them for you.
## Gradle automatically packages shared libraries with your APK.
#
#add_library( # Sets the name of the library.
#             native-lib
#
#             # Sets the library as a shared library.
#             SHARED
#
#             # Provides a relative path to your source file(s).
#             native-lib.cpp )
#
## Searches for a specified prebuilt library and stores the path as a
## variable. Because CMake includes system libraries in the search path by
## default, you only need to specify the name of the public NDK library
## you want to add. CMake verifies that the library exists before
## completing its build.
#
#find_library( # Sets the name of the path variable.
#              log-lib
#
#              # Specifies the name of the NDK library that
#              # you want CMake to locate.
#              log )
#
## Specifies libraries CMake should link to your target library. You
## can link multiple libraries, such as libraries you define in this
## build script, prebuilt third-party libraries, or system libraries.
#
#target_link_libraries( # Specifies the target library.
#                       native-lib
#
#                       # Links the target library to the log library
#                       # included in the NDK.
#                       ${log-lib} )

cmake_minimum_required(VERSION 3.22.1)

execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/version.sh -s print
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE TINYALSA_VERSION)

project("tinyalas")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(TINYALSA_USES_PLUGINS "Whether or not to build with plugin support" OFF)
option(TINYALSA_BUILD_EXAMPLES "Build examples" ON)
option(TINYALSA_BUILD_UTILS "Build utility tools" ON)

# Library
add_library("tinyalsa"
        "pcm_reader.cpp"
        "tinyalsa_jni.cpp"
        "src/pcm.c"
        "src/pcm_hw.c"
        "src/pcm_plugin.c"
        "src/snd_card_plugin.c"
        "src/mixer.c"
        "src/mixer_hw.c"
        "src/mixer_plugin.c")

set_property(TARGET "tinyalsa" PROPERTY PUBLIC_HEADER
        "include/tinyalsa/attributes.h"
        "include/tinyalsa/version.h"
        "include/tinyalsa/asoundlib.h"
        "include/tinyalsa/pcm.h"
        "include/tinyalsa/plugin.h"
        "include/tinyalsa/mixer.h"
        "pcm_reader.h"
)
find_library(
        log-lib
        log
)
set_target_properties("tinyalsa" PROPERTIES
        VERSION ${TinyALSA_VERSION}
        SOVERSION ${TinyALSA_VERSION_MAJOR})

target_include_directories("tinyalsa" PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_compile_definitions("tinyalsa" PRIVATE
        $<$<BOOL:${TINYALSA_USES_PLUGINS}>:TINYALSA_USES_PLUGINS>
        PUBLIC _POSIX_C_SOURCE=200809L)
target_link_libraries("tinyalsa" PUBLIC ${CMAKE_DL_LIBS}
        ${log-lib}
)

# Examples
#if(TINYALSA_BUILD_EXAMPLES)
#    set(TINYALSA_EXAMPLES pcm-readi pcm-writei)
#else()
#    set(TINYALSA_EXAMPLES)
#endif()
#
#foreach(EXAMPLE IN LISTS TINYALSA_EXAMPLES)
#    add_executable("${EXAMPLE}" "examples/${EXAMPLE}.c")
#    target_link_libraries("${EXAMPLE}" PRIVATE "tinyalsa")
#endforeach()

# Utilities
#if(TINYALSA_BUILD_UTILS)
#    set(TINYALSA_UTILS tinyplay tinycap tinypcminfo tinymix tinywavinfo)
#else()
#    set(TINYALSA_UTILS)
#endif()

#foreach(UTIL IN LISTS TINYALSA_UTILS)
#    add_executable("${UTIL}" "utils/${UTIL}.c")
#    target_link_libraries("${UTIL}" PRIVATE "tinyalsa")
#endforeach()
#
#if(TINYALSA_BUILD_UTILS)
#    target_link_libraries("tinywavinfo" PRIVATE m)
#endif()

# Add C warning flags
#include(CheckCCompilerFlag)
#foreach(FLAG IN ITEMS -Wall -Wextra -Wpedantic -Werror -Wfatal-errors)
#    string(TOUPPER "HAVE${FLAG}" HAVE_VAR)
#    string(REPLACE "-" "_" HAVE_VAR "${HAVE_VAR}")
#    check_c_compiler_flag("${FLAG}" "${HAVE_VAR}")
#    if("${${HAVE_VAR}}")
#        target_compile_options("tinyalsa" PRIVATE "${FLAG}")
#        foreach(UTIL IN LISTS TINYALSA_UTILS)
#            target_compile_options("${UTIL}" PRIVATE "${FLAG}")
#        endforeach()
#    endif()
#endforeach()

# Install
include(GNUInstallDirs)
install(TARGETS "tinyalsa" ${TINYALSA_UTILS}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tinyalsa)
