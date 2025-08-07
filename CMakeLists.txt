cmake_minimum_required(VERSION 3.30)
project("Engine" VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include libraries
include(cmake/third_party.cmake)

# Add subdirectories
add_subdirectory(src/engine)
add_subdirectory(src/app)

# LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/compile_commands.json"
            "${CMAKE_SOURCE_DIR}/compile_commands.json"
    DEPENDS  "${CMAKE_BINARY_DIR}/compile_commands.json"
    COMMENT "Sync compile_commands.json to project root"
)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
	message(FATAL_ERROR "You cannot build in a source directory (or any directory with "
		"CMakeLists.txt file). Please make a build subdirectory.")
endif()
