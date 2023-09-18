#
# src/CMakeLists.txt
#

# Source files
set(SOURCES
    exports.def

    factory/factory.cpp
    factory/factory.h

    handler/handler.cpp
    handler/handler.h

    main/dllmain.cpp
    main/config.h

    registry/registry.cpp
    registry/registry.h

    util/util.cpp
    util/util.h
    
    resources/resource.h
    resources/resource.rc
)

# Executable
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Warnings
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    shlwapi.lib
    windowscodecs.lib
)