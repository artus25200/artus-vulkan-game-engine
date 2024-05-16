/*
**   Artus25200's Vulkan Game Engine (AVGE)
*/

#ifndef AVGE_H_
#define AVGE_H_
#include <GLFW/glfw3.h>
#include <nicelog.h>
#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>
#define AVGE_VERSION(major, minor, patch) VK_MAKE_VERSION(major, minor, patch)

typedef enum { AVGE_ERROR = 0, AVGE_OK = 1, AVGE_WARN = 2 } AVGEStatusCode;
typedef enum { AVGE_EXIT_SUCCESS, AVGE_EXIT_FAILURE } AVGEExitCode;

#ifndef NO_MALLOC_REDEF
#define malloc(size) AVGE_malloc(size, __FILE__, __LINE__, __func__);
#define free(ptr) AVGE_free(ptr)
#endif

typedef struct App {
  char *name;
  uint32_t version;

  Logger *logger;

  // GLFW
  GLFWwindow *glfw_window;

  // VULKAN
  VkApplicationInfo vulkan_application_info;
  VkInstance vulkan_instance;

  VkDevice vulkan_device;

  VkSurfaceKHR vulkan_surface;
} App;

/***************************
** ENGINE
***************************/

AVGEStatusCode AVGE_initialize_engine(App *app);
void AVGE_terminate_engine();

/***************************
** MEMORY
***************************/
void *AVGE_malloc(size_t size, char *file, uint line, const char *func);
void AVGE_free(void *ptr);
void AVGE_free_all();
void AVGE_print_memory_debug();

/***************************
** APP
***************************/

App *AVGE_create_application(char *name, uint32_t version);
Logger *AVGE_get_app_logger(App *app);

/**************************
** VULKAN
**************************/

AVGEStatusCode AVGE_initialize_vulkan(App *app);

/**************************
** GLFW
**************************/

AVGEStatusCode AVGE_initialize_glfw();
AVGEStatusCode AVGE_create_window(App *app, int width, int height, char *name);

#endif // AVGE_H_
