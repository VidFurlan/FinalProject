#define GLFW_INCLUDE_VULKAN
#include "Window.hpp"

#include "VulkanRenderer.hpp"
#include "debug/Debug.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_funcs.hpp>

namespace engine::renderer {
	const std::vector<const char *> VulkanRenderer::validationLayers = {
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_LUNARG_standard_validation",
	};

	void VulkanRenderer::init(const RenderInitInfo &info) {
		m_window = static_cast<GLFWwindow *>(info.windowHandle);
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
	}

	void VulkanRenderer::beginFrame() {
	}

	void VulkanRenderer::draw() {
	}

	void VulkanRenderer::endFrame() {
	}

	void VulkanRenderer::resize(uint32_t w, uint32_t h) {
	}

	void VulkanRenderer::waitIdle() {
	}

	void VulkanRenderer::createInstance() {
		constexpr vk::ApplicationInfo appInfo(
			"Hello Triangle",
			VK_MAKE_VERSION(1, 0, 0),
			"No Engine",
			VK_MAKE_VERSION(1, 0, 0),
			vk::ApiVersion14);

		// Get the required layers
		std::vector<char const *> requiredLayers;
		if (enableValidationLayers) {
			requiredLayers.assign(validationLayers.begin(), validationLayers.end());
		}

		// Check if the required layers are supported by the Vulkan implementation.
		auto layerProperties = context.enumerateInstanceLayerProperties();
		for (auto const &requiredLayer : requiredLayers) {
			if (std::ranges::none_of(layerProperties,
									 [requiredLayer](auto const &layerProperty) { return strcmp(layerProperty.layerName, requiredLayer) == 0; })) {
				throw std::runtime_error("Required layer not supported: " + std::string(requiredLayer));
			}
		}

		// Get the required extensions.
		auto requiredExtensions = getRequiredExtensions();

		// Check if the required extensions are supported by the Vulkan implementation.
		auto extensionProperties = context.enumerateInstanceExtensionProperties();
		for (auto const &requiredExtension : requiredExtensions) {
			if (std::ranges::none_of(extensionProperties,
									 [requiredExtension](auto const &extensionProperty) { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; })) {
				throw std::runtime_error("Required extension not supported: " + std::string(requiredExtension));
			}
		}

		vk::InstanceCreateInfo createInfo(
			{},
			&appInfo,
			static_cast<uint32_t>(requiredLayers.size()),
			requiredLayers.data(),
			static_cast<uint32_t>(requiredExtensions.size()),
			requiredExtensions.data());
		instance = vk::raii::Instance(context, createInfo);
	}

	void VulkanRenderer::setupDebugMessenger() {
		if (!enableValidationLayers) return;

		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT(
			{},
			severityFlags,
			messageTypeFlags,
			&vulkanDebugCallback);
		debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
	}

	void VulkanRenderer::createSurface() {
		VkSurfaceKHR _surface;
		if (glfwCreateWindowSurface(*instance, m_window, nullptr, &_surface) != 0) {
			throw std::runtime_error("failed to create window surface!");
		}
		surface = vk::raii::SurfaceKHR(instance, _surface);
	}

	void VulkanRenderer::pickPhysicalDevice() {
		std::vector<vk::raii::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
		const auto devIter = std::ranges::find_if(
			devices,
			[&](auto const &device) {
				// Check if the device supports the Vulkan 1.3 API version
				bool supportsVulkan1_3 = device.getProperties().apiVersion >= VK_API_VERSION_1_3;

				// Check if any of the queue families support graphics operations
				auto queueFamilies = device.getQueueFamilyProperties();
				bool supportsGraphics =
					std::ranges::any_of(queueFamilies, [](auto const &qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });

				// Check if all required device extensions are available
				auto availableDeviceExtensions = device.enumerateDeviceExtensionProperties();
				bool supportsAllRequiredExtensions =
					std::ranges::all_of(requiredDeviceExtension,
										[&availableDeviceExtensions](auto const &requiredDeviceExtension) {
											return std::ranges::any_of(availableDeviceExtensions,
																	   [requiredDeviceExtension](auto const &availableDeviceExtension) { return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
										});

				auto features = device.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
				bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
												features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

				return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
			});
		if (devIter != devices.end()) {
			physicalDevice = *devIter;
		} else {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	void VulkanRenderer::createLogicalDevice() {
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

		// get the first index into queueFamilyProperties which supports both graphics and present
		uint32_t queueIndex = ~0;
		for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++) {
			if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
				physicalDevice.getSurfaceSupportKHR(qfpIndex, *surface)) {
				// found a queue family that supports both graphics and present
				queueIndex = qfpIndex;
				break;
			}
		}
		if (queueIndex == ~0) {
			throw std::runtime_error("Could not find a queue for graphics and present -> terminating");
		}

		// query for Vulkan 1.3 features
		vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain;
		featureChain.get<vk::PhysicalDeviceVulkan13Features>()
			.setDynamicRendering(VK_TRUE);
		featureChain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
			.setExtendedDynamicState(VK_TRUE);

		// create a Device
		float queuePriority = 0.0f;
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo(
			{},
			queueIndex,
			1,
			&queuePriority);
		vk::DeviceCreateInfo deviceCreateInfo(
			{},
			1,
			&deviceQueueCreateInfo,
			{},
			{},
			static_cast<uint32_t>(requiredDeviceExtension.size()),
			requiredDeviceExtension.data(),
			{},
			&featureChain.get<vk::PhysicalDeviceFeatures2>());

		device = vk::raii::Device(physicalDevice, deviceCreateInfo);
		queue = vk::raii::Queue(device, queueIndex, 0);
	}

	std::vector<const char *> VulkanRenderer::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back(vk::EXTDebugUtilsExtensionName);
		}

		return extensions;
	}

	static VKAPI_ATTR vk::Bool32 VKAPI_CALL vulkanDebugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type, const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData, void *) {
		if (severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError || severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning) {
			ENGINE_LOG_DEBUG("VULKAN: validation layer: type " + to_string(type) + " msg: " + pCallbackData->pMessage);
		}

		return vk::False;
	}
} // namespace engine::renderer
