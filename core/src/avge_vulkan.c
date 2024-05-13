#include "nicelog.h"
#include <avge.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#ifdef OSX
#define GLFW_INCLUDE_VULKAN
#else
#include <vulkan/vulkan.h>
#endif
#include <GLFW/glfw3.h>

static const char *validationLayers[] = {"VK_LAYER_KHRONOS_validation", NULL};

#ifdef ENGINE_DEBUG
static const bool enableValidationLayers = true;
#else
static const bool enableValidationLayers = false;
#endif

static uint32_t deviceExtensionCount = 0;
static const char *deviceExtension[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, NULL};

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *data, void *userData) {
  int _severity = 0;
  while ((severity & 0xF) != 0x1) {
    _severity++;
    severity >>= 4;
  }
  NL_log_output(NULL, _severity + 2, __FILE__, __LINE__,
                "Validation Layer : %s", data->pMessage);
  return VK_FALSE;
}

void check_validation_layers(App *app) {
  INFO(AVGE_get_app_logger(app), "Checking Validation Layers...");
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);
  VkLayerProperties *availableLayers =
      malloc(sizeof(VkLayerProperties) * layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  int i = 0;
  while (validationLayers[i] != NULL) {
    bool layerFound = false;
    for (int j = 0; j < layerCount; ++j) {
      if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
        layerFound = true;
      }
      if (!layerFound) {
        FATAL(AVGE_get_app_logger(app),
              "Validation Layer \"%s\" not available !", validationLayers[i]);
      }
    }
    ++i;
  }
}

struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
};

struct QueueFamilyIndices findQueueFamily(VkPhysicalDevice device,
                                          VkSurfaceKHR surface) {
  struct QueueFamilyIndices indices = {-1, -1};

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
  VkQueueFamilyProperties *queueFamilies =
      malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies);
  for (int i = 0; i < queueFamilyCount; ++i) {
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }
    if (surface == NULL)
      continue;
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport)
      indices.presentFamily = i;
  }
  return indices;
}

bool isDeviceOK(VkPhysicalDevice device) {
  deviceExtensionCount = 0;
  while (deviceExtension[deviceExtensionCount] != NULL)
    ++deviceExtensionCount;

  struct QueueFamilyIndices indices = findQueueFamily(device, NULL);
  if (indices.graphicsFamily == -1)
    return false;
  uint32_t availableExtensionCount;
  vkEnumerateDeviceExtensionProperties(device, NULL, &availableExtensionCount,
                                       NULL);
  VkExtensionProperties *availableExtensions =
      malloc(sizeof(VkExtensionProperties) * availableExtensionCount);
  vkEnumerateDeviceExtensionProperties(device, NULL, &availableExtensionCount,
                                       availableExtensions);
  for (int i = 0; i < deviceExtensionCount; ++i) {
    bool found = false;
    for (int j = 0; j < availableExtensionCount; ++j) {
      if (strcmp(deviceExtension[i], availableExtensions[j].extensionName) ==
          0) {
        found = true;
        break;
      }
    }
    if (found)
      continue;
    return false;
  }
  return true;
}
void get_best_physical_device(VkPhysicalDevice *bestDevice,
                              VkPhysicalDevice *devices, uint32_t count) {
  int bestScore = 0;
  uint32_t bestDeviceIndex = -1;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceFeatures features;
  for (int i = 0; i < count; i++) {
    if (!isDeviceOK(devices[i]))
      continue;
    int score = 0;
    vkGetPhysicalDeviceProperties(devices[i], &properties);
    vkGetPhysicalDeviceFeatures(devices[i], &features);
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      score += 1000;
    score += properties.limits.maxImageDimension2D;
    if (score > bestScore) {
      bestScore = score;
      bestDeviceIndex = i;
    }
  }
  if (bestDeviceIndex != -1)
    *bestDevice = devices[bestDeviceIndex];
}

AVGEStatusCode AVGE_initialize_vulkan() {
  BEGIN("Initializing Vulkan");

  return AVGE_ERROR; // TODO: NOT IMPLEMENTED
}
