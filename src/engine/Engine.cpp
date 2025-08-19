#include "Engine.hpp"
#include "debug/Debug.hpp"

namespace engine {
	Engine::Engine(std::unique_ptr<renderer::IRenderer> renderer)
		: m_renderer(std::move(renderer)), m_isRunning(false) {}

	void Engine::init(void *windowHandle, uint32_t width, uint32_t height, bool vsync) {
		renderer::RenderInitInfo info{windowHandle, width, height, vsync};
		m_renderer->init(info);
		m_isRunning = true;
	}

	void Engine::update() {
		ENGINE_LOG_DEBUG("Updating engine state");
	}

	void Engine::render() {
		if (m_isRunning) {
			m_renderer->beginFrame();
			m_renderer->draw();
			m_renderer->endFrame();
		}
	}

	void Engine::resize(uint32_t width, uint32_t height) {
		m_renderer->resize(width, height);
	}

	void Engine::shutdown() {
		m_isRunning = false;
		if (m_renderer) m_renderer->waitIdle();
	}
} // namespace engine
