#
# CMakeLists.txt
#

cmake_minimum_required(VERSION 3.6)

# Global
set(PROJECT_NAME RageShellIconHandler)
project(${PROJECT_NAME} VERSION 1.0)

# Configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configurations" FORCE)

# Properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Runtime
if (MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
endif()

# Unicode Charset
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

# Directories
add_subdirectory(src)