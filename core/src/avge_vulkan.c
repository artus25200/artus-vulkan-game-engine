#include "avge_internal.h"
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
#include <avge_internal.h>

static const char *validation_layers[] = {"VK_LAYER_KHRONOS_validation", NULL};

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

AVGEStatusCode check_validation_layers(App *app, int *validation_layer_count) {
  INFO(AVGE_state.logger, "Checking Vulkan Validation Layers...");
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);
  VkLayerProperties *availableLayers =
      malloc(sizeof(VkLayerProperties) * layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  int i = 0;
  while (validation_layers[i] != NULL) {
    bool layerFound = false;
    for (int j = 0; j < layerCount; ++j) {
      if (strcmp(validation_layers[i], availableLayers[j].layerName) == 0) {
        layerFound = true;
      }
      if (!layerFound) {
        FATAL(AVGE_state.logger, "Validation Layer \"%s\" not available !",
              validation_layers[i]);
        return AVGE_ERROR;
      }
    }
    ++i;
  }
  *validation_layer_count = i;
  return AVGE_OK;
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

char **get_extensions(bool enable_validation_layers, int *extension_count) {
  uint32_t engineExtensionCount = 0;
  const char *engineExtension[] = {
      VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
      enable_validation_layers ? VK_EXT_DEBUG_UTILS_EXTENSION_NAME : NULL,
      NULL};

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;

  while (engineExtension[engineExtensionCount] != NULL) {
    ++engineExtensionCount;
  }

  int total_extension_count = glfwExtensionCount + engineExtensionCount;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  char **Extensions = malloc(sizeof(char *) * total_extension_count);

  memcpy(Extensions, engineExtension, sizeof(char *) * engineExtensionCount);
  memcpy(Extensions + engineExtensionCount, glfwExtensions,
         sizeof(char *) * glfwExtensionCount);
  *extension_count = total_extension_count;
  return Extensions;
}

AVGEStatusCode create_debug_callback(bool enable_validation_layers,
                                     VkInstance instance) {
  int error_code;
  PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyUtilsMessengerEXT;
  VkDebugUtilsMessengerEXT debugMessenger;
  if (enable_validation_layers) {
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {0};
    messengerCreateInfo.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerCreateInfo.messageSeverity = 0x1111;
    messengerCreateInfo.messageType = 1 | 2 | 4;
    messengerCreateInfo.pfnUserCallback = debug_callback;
    messengerCreateInfo.pUserData = NULL;
    PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugUtilsMessengerEXT");
    pfnVkDestroyUtilsMessengerEXT =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT");
    if ((error_code = pfnVkCreateDebugUtilsMessengerEXT(
             instance, &messengerCreateInfo, NULL, &debugMessenger)) !=
        VK_SUCCESS) {
      FATAL(AVGE_state.logger,
            "Could not create debug messenger ! error code : %d", error_code);
      return AVGE_ERROR;
    }
  }
  return AVGE_OK;
}

AVGEStatusCode AVGE_initialize_vulkan(App *app) {
  BEGIN("Initializing Vulkan");
  int status = AVGE_OK;

  uint32_t engine_version = VK_MAKE_VERSION(1, 0, 0);
  uint32_t app_version = app->version;
  VkApplicationInfo application_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                        NULL,
                                        app->name,
                                        app_version,
                                        "AVGE",
                                        engine_version,
                                        VK_API_VERSION_1_3};

  int validation_layer_count = 0;
  bool enable_validation_layers = NL_get_log_level(app->logger) <= NL_DEBUG;
  if (enable_validation_layers)
    status = check_validation_layers(app, &validation_layer_count);
  if (status != AVGE_OK)
    return status;

  VkInstanceCreateInfo instance_create_info = {
      VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      NULL,
      0,
      &application_info,
      enable_validation_layers ? validation_layer_count : 0,
      enable_validation_layers ? validation_layers : NULL,
      0,
      NULL};
  instance_create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

  int extension_count;
  char **extensions =
      get_extensions(enable_validation_layers, &extension_count);

  instance_create_info.enabledExtensionCount = extension_count;
  instance_create_info.ppEnabledExtensionNames = (const char **)extensions;

  if ((status = vkCreateInstance(&instance_create_info, NULL,
                                 &app->vulkan_instance)) != VK_SUCCESS) {
    FATAL(AVGE_state.logger, "Could not create VkInstance, error code : %d",
          status);
    DONE(NL_ABORTED);
    return AVGE_ERROR;
  }

  if (!create_debug_callback(enable_validation_layers, app->vulkan_instance))
    return AVGE_ERROR;

  AVGE_state.vulkan_initialized = true;
  INFO(AVGE_state.logger, "Successfully initialized Vulkan");
  DONE(NL_OK);
  return AVGE_ERROR; // TODO: NOT IMPLEMENTED
}
