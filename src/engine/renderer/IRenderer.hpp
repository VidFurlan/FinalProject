#pragma once

#include <cstdint>

namespace engine::renderer {
	struct RenderInitInfo {
		void *windowHandle;
		uint32_t width = 0, height = 0;
		bool vsync = false;
	};

	class IRenderer {
	  public:
		virtual ~IRenderer() = default;
		virtual void init(const RenderInitInfo &info) = 0;
		virtual void beginFrame() = 0;
		virtual void draw() = 0;
		virtual void endFrame() = 0;
		virtual void resize(uint32_t w, uint32_t h) = 0;
		virtual void waitIdle() = 0;
	};
} // namespace engine::renderer
