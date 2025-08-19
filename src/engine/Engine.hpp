#pragma once

#include "renderer/IRenderer.hpp"
#include <memory>

namespace engine {
	class Engine {
	  public:
		explicit Engine(std::unique_ptr<renderer::IRenderer> renderer);
		~Engine() = default;

		void init(void *windowHandle, uint32_t width, uint32_t height, bool vsync = false);
		void update();
		void render();
		void resize(uint32_t width, uint32_t height);
		void shutdown();

	  private:
		std::unique_ptr<renderer::IRenderer> m_renderer;
		bool m_isRunning = false;
	};

} // namespace engine
