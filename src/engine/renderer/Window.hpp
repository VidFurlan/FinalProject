#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>

namespace engine::renderer {
	class Window {
	  public:
		using ResizeCallback = void (*)(uint32_t width, uint32_t height);

		bool create(uint32_t width, uint32_t height, const char *title);
		void destroy();
		void poll();
		bool shouldClose() const;
		GLFWwindow *getHandle() const;
		void onResize(ResizeCallback callback);

	  private:
		GLFWwindow *m_window = nullptr;
		ResizeCallback m_resizeCallback = nullptr;
	};
} // namespace engine::renderer
