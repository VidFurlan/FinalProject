#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "IRenderer.hpp"

namespace engine::renderer {
	class VulkanRenderer : public IRenderer {
	  public:
		void init(const RenderInitInfo &info) override;
		void beginFrame() override;
		void draw() override;
		void endFrame() override;
		void resize(uint32_t width, uint32_t height) override;
		void waitIdle() override;

	  private:
		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();

		std::vector<const char *> getRequiredExtensions();

		GLFWwindow *m_window = nullptr;
		std::vector<const char *> requiredDeviceExtension = {
			vk::KHRSwapchainExtensionName,
			vk::KHRSpirv14ExtensionName,
			vk::KHRSynchronization2ExtensionName,
			vk::KHRCreateRenderpass2ExtensionName};
		vk::raii::Context context;
		vk::raii::Instance instance = nullptr;
		vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
		vk::raii::SurfaceKHR surface = nullptr;
		vk::raii::PhysicalDevice physicalDevice = nullptr;
		vk::raii::Device device = nullptr;
		vk::raii::Queue queue = nullptr;

		static const std::vector<const char *> validationLayers;

#ifdef NDEBUG
		constexpr static bool enableValidationLayers = false;
#else
		constexpr static bool enableValidationLayers = true;
#endif
	};

	static VKAPI_ATTR vk::Bool32 VKAPI_CALL vulkanDebugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type, const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData, void *);
} // namespace engine::renderer
