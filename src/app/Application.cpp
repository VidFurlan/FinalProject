#include "Application.hpp"
#include "debug/Debug.hpp"
#include "renderer/vulkan/VulkanRenderer.hpp"
#include <memory>

Application::Application()
	: m_engine(std::make_unique<engine::renderer::VulkanRenderer>()) {}

Application::~Application() {
	m_window.destroy();
}

int Application::run() {
	if (!m_window.create(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Engine")) {
		ENGINE_LOG_FATAL("Window creation failed");
		return -1;
	}

	m_engine.init(m_window.getHandle(), DEFAULT_WIDTH, DEFAULT_HEIGHT, true);
	loop();
	m_engine.shutdown();
	return 0;
}

void Application::loop() {
	while (!m_window.shouldClose()) {
		m_window.poll();

		if (m_paused) {
			continue;
		}

		m_engine.update();
		m_engine.render();
	}
}

void Application::cleanup() {
	m_engine.shutdown();
	m_window.destroy();
}
