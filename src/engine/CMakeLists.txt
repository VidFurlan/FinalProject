set(ENGINE_SRC
	engine.hpp
	engine.cpp
	debug/debug.hpp
	debug/debug.cpp
	debug/debug_init.cpp
)

add_library(engine STATIC ${ENGINE_SRC} "debug/debug_init.cpp")
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(engine
	PUBLIC
	glfw
	glm
	stb
	tinyobj
	Vulkan::Vulkan
)

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