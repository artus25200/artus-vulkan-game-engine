/*
**   Artus25200's Vulkan Game Engine (AVGE)
*/

#ifndef AVGE_H_
#define AVGE_H_
#include <nicelog.h>
#include <stdint.h>
#include <vulkan/vulkan_core.h>

typedef enum { AVGE_ERROR = 0, AVGE_OK = 1, AVGE_WARN = 2 } AVGEStatusCode;
typedef enum { AVGE_EXIT_SUCCESS, AVGE_EXIT_FAILURE } AVGEExitCode;

#define AVGE_VERSION(major, minor, patch) VK_MAKE_VERSION(major, minor, patch)
/***************************
** CORE
***************************/

// --- ENGINE --- //
AVGEStatusCode AVGE_initialize(void);

// --- APP --- //
typedef struct App {
  char *name;
  uint32_t version;

  Logger *logger;

  // VULKAN
  VkApplicationInfo vulkan_application_info;
  VkInstance vulkan_instance;

  VkDevice vulkan_device;
} App;

App *AVGE_create_application(char *name, uint32_t version);
Logger *AVGE_get_app_logger(App *app);

/**************************
** VULKAN
**************************/

AVGEStatusCode AVGE_initialize_vulkan(App *app);

#endif // AVGE_H_
