#pragma once

#include "Engine.hpp"
#include "renderer/Window.hpp"

class Application {
	constexpr static uint32_t DEFAULT_WIDTH = 1280;
	constexpr static uint32_t DEFAULT_HEIGHT = 720;

  public:
	Application();
	~Application();
	int run();
	void cleanup();

  private:
	void loop();

	engine::renderer::Window m_window;
	engine::Engine m_engine;
	bool m_paused = false;
};
