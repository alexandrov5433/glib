cmake_minimum_required(VERSION 3.20)

project(glib VERSION 1.0.0 LANGUAGES C)

add_library(glib SHARED)

target_sources(glib
    PRIVATE
        src/linkedList.c
        src/hashMap.c
        src/sorting.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/linkedList.h
            include/hashMap.h
            include/sorting.h
)




# executable for testing
add_executable(glib_test glib_test.c)
target_include_directories(glib_test PRIVATE include)
target_link_directories(glib_test PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(glib_test glib)
# compile lib code with -g flag, enabling debugging of glib_test.exe with working breakpoints
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build." FORCE)
endif()




# for managing import/export on windows; see .h files
# when compiling: GLIB_EXPORTS is defined and used in the macro in the .h files to mark the funktions for exporting.
# when importing the .dll: GLIB_EXPORTS is not defined, so the functions are marked for importing. This way the linker in windows knows where the used functions are to be found.
# this is relevant only for windows !!!
target_compile_definitions(glib PRIVATE GLIB_EXPORTS)

# Public include directories for consumers
target_include_directories(glib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(glib PROPERTIES
    OUTPUT_NAME "glib"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

include(GNUInstallDirs)

install(TARGETS glib
    EXPORT glibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT glibTargets
    FILE glibTargets.cmake
    NAMESPACE glib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glib
)