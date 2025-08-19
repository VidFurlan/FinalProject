set(RENDERER_SRC
	Window.cpp
	Window.hpp
	IRenderer.hpp
	vulkan/VulkanRenderer.cpp
	vulkan/VulkanRenderer.hpp
)

add_library(renderer STATIC ${RENDERER_SRC})
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(renderer PUBLIC engine)
