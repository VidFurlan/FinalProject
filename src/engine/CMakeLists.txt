set(ENGINE_SRC
	Engine.hpp
	Engine.cpp
	debug/Debug.hpp
	debug/Debug.cpp
)

# Libraries
add_library(engine STATIC ${ENGINE_SRC})
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(engine
	PUBLIC
	glfw
	glm
	stb
	tinyobj
	Vulkan::Vulkan
)

# Platform-specific Vulkan
if(WIN32)
	target_compile_definitions(engine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(APPLE)
	target_compile_definitions(engine PRIVATE VK_USE_PLATFORM_METAL_EXT
		VK_ENABLE_BETA_EXTENSIONS)
else()
	target_compile_definitions(engine PRIVATE VK_USE_PLATFORM_XLIB_KHR
                                            VK_USE_PLATFORM_WAYLAND_KHR)
endif()

add_subdirectory(renderer)
