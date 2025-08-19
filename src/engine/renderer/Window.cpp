#include "Window.hpp"
#include <algorithm>

namespace engine::renderer {
	bool Window::create(uint32_t width, uint32_t height, const char *title) {
		if (!glfwInit()) return false;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		return m_window != nullptr;
	}
	void Window::destroy() {
		if (m_window) {
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
		glfwTerminate();
		if (m_resizeCallback) {
			m_resizeCallback = nullptr;
		}
	}
	void Window::poll() {
		glfwPollEvents();
	}
	bool Window::shouldClose() const {
		return m_window ? glfwWindowShouldClose(m_window) : true;
	}
	GLFWwindow *Window::getHandle() const {
		return m_window;
	}
	void Window::onResize(ResizeCallback callback) {
		m_resizeCallback = std::move(callback);
		if (m_window) {
			glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
				auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
				if (self && self->m_resizeCallback) {
					self->m_resizeCallback(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
				}
			});
		}
	}
} // namespace engine::renderer
