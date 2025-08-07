include(FetchContent)

# For CMake FindPackage
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# ---------- Vulkan ----------
find_package(Vulkan CONFIG QUIET)
if(NOT TARGET Vulkan::Vulkan)
	find_package(Vulkan REQUIRED)
endif()
include(cmake/FindVulkan.cmake)

# ------- FetchContent -------
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG        3.4
)
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG        1.0.1
)
FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG        master
)
FetchContent_Declare(
	tinyobj 
	GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git         
	GIT_TAG        v2.0.0rc13
)

FetchContent_MakeAvailable(glfw glm stb tinyobj)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

# ---------- tinyobjloader ----------
if (TARGET tinyobjloader)              # when using an old 1.x tag
    add_library(tinyobj INTERFACE)
    target_include_directories(tinyobj INTERFACE ${tinyobj_SOURCE_DIR})
else()                                 # modern tag exports proper target
    add_library(tinyobj ALIAS tinyobjloader::tinyobjloader)
endif()

