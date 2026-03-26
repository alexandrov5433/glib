cmake_minimum_required(VERSION 3.20)

project(galxlib VERSION 1.0.0 LANGUAGES C)

add_library(galxlib STATIC)


# Linux
# Static library (libgalxlib.a) -> /usr/lib
# Headers (*.h) -> /usr/include/galxlib
# Include -> #include "galxlib/***.h"    (#include "galxlib/hash_map.h")
# Linker configuration
# 'touch /etc/ld.so.conf.d/galxlib.conf'   with content:  /usr/lib  (this is the path to the archive)
# Run 'sudo ldconfig' (not sure if it a must, because it updates cache for shared libraries)

# When using galxlib, the project must be compiled with linker flags -lgalxlib -lm: 'gcc test.c -o test -lgalxlib -lm'


# Stackoverflow: https://stackoverflow.com/questions/34625627/how-to-link-to-the-c-math-library-with-cmake
# Many mathematical functions (pow, sqrt, fabs, log etc.)
# are declared in math.h and require the library libm to be linked.
# Unlike libc, which is automatically linked, libm is a separate library and often requires explicit linkage. 
# The linker presumes all libraries to begin with lib, so to link to libm you link to m.
# libm (Wikipedia): https://en.wikipedia.org/wiki/C_mathematical_functions#libm
target_link_libraries(galxlib PUBLIC m)

# https://cmake.org/cmake/help/latest/command/target_link_libraries.html
# PUBLIC - Populates both properties for building and properties for using a target.
# PRIVATE - Populates only properties for building a target.
# INTERFACE - Populates only properties for using a target.

# https://cmake.org/cmake/help/latest/module/CheckLibraryExists.html
include(CheckLibraryExists)
check_library_exists(regex regcomp "" HAVE_LIBREGEX)

if (HAVE_LIBREGEX)
    target_link_libraries(meval PUBLIC regex)
endif()

target_sources(galxlib
    PRIVATE
        src/data_structures/linkedList.c
        src/data_structures/hash_map.c
        src/sorting.c
        src/data_structures/binary_tree.c
        src/data_structures/stack.c
        src/data_structures/dynamic_array.c
        src/gstring.c
        src/gregex.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/data_structures/linkedList.h
            include/data_structures/hash_map.h
            include/sorting.h
            include/data_structures/binary_tree.h
            include/data_structures/stack.h
            include/data_structures/dynamic_array.h
            include/gstring.h
            include/gregex.h
)




# executable for testing
add_executable(lib_test lib_test/main.c)
target_include_directories(lib_test PRIVATE include)
target_link_directories(lib_test PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(lib_test galxlib)
target_sources(lib_test 
    PRIVATE
        lib_test/src/dynamic_array.c
        lib_test/src/gstring.c
        lib_test/src/hash_map.c
        lib_test/src/sorting.c
        lib_test/src/stack.c
        lib_test/src/linked_list.c
        lib_test/src/gregex.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            lib_test/include
        FILES
            lib_test/include/dynamic_array.h
            lib_test/include/gstring.h
            lib_test/include/hash_map.h
            lib_test/include/sorting.h
            lib_test/include/stack.h
            lib_test/include/linked_list.h
            lib_test/include/gregex.h

)
# compile lib code with -g flag, enabling debugging of lib_test.exe with working breakpoints
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build." FORCE)
endif()




# for managing import/export on windows; see .h files
# when compiling: GLIB_EXPORTS is defined and used in the macro in the .h files to mark the funktions for exporting.
# when importing the .dll: GLIB_EXPORTS is not defined, so the functions are marked for importing. This way the linker in windows knows where the used functions are to be found.
# this is relevant only for windows !!!
target_compile_definitions(galxlib PRIVATE GALXLIB_EXPORTS)

# Public include directories for consumers
target_include_directories(galxlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(galxlib PROPERTIES
    OUTPUT_NAME "galxlib"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

include(GNUInstallDirs)

install(TARGETS galxlib
    EXPORT galxlibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT galxlibTargets
    FILE galxlibTargets.cmake
    NAMESPACE galxlib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/galxlib
)