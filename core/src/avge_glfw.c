#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#ifdef OSX
#define GLFW_INCLUDE_VULKAN
#else
#include <vulkan/vulkan.h>
#endif
#include "avge_internal.h"
#include <GLFW/glfw3.h>
#include <avge.h>
#include <nicelog.h>

AVGEStatusCode AVGE_create_window(App *app, int width, int height, char *name) {
  if (!AVGE_state.glfw_initialized) {
    FATAL(AVGE_state.logger,
          "You must initialize GLFW first ! Use AVGE_initialize_engine().");
    return AVGE_ERROR;
  }
  if (!AVGE_state.vulkan_initialized) {
    FATAL(AVGE_state.logger,
          "Vulkan must be initialized first ! Use AVGE_initialize_engine().");
    return AVGE_ERROR;
  }
  int error_code;
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef OSX
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // Mac
#endif                                                   // OSX
  app->glfw_window = glfwCreateWindow(800, 600, "Vulkan Engine",
                                      glfwGetPrimaryMonitor(), NULL);
  if (!app->glfw_window) {
    FATAL(AVGE_state.logger, "Could not create window ! error code : %d",
          glfwGetError(NULL));
    return AVGE_ERROR;
  }
  if ((error_code =
           glfwCreateWindowSurface(app->vulkan_instance, app->glfw_window, NULL,
                                   &app->vulkan_surface)) != VK_SUCCESS) {
    const char *err;
    glfwGetError(&err);
    FATAL(AVGE_state.logger,
          "Could not create window surface ! Error code: %d. GLFW Error: %s",
          error_code, err);
    return AVGE_ERROR;
  }
  glfwShowWindow(app->glfw_window);
  return AVGE_OK;
}

AVGEStatusCode AVGE_initialize_glfw() {
  BEGIN("Initializing GLFW");
  if (glfwInit() != GLFW_TRUE) {
    FATAL(AVGE_state.logger, "Could not initialize GLFW ! error code : %d",
          glfwGetError(NULL));
    DONE(NL_ABORTED);
    return AVGE_ERROR;
  }
  if (glfwVulkanSupported() != GLFW_TRUE) {
    FATAL(AVGE_state.logger, "Vulkan is not supported by GLFW");
    DONE(NL_ABORTED);
    return AVGE_ERROR;
  }
  INFO(AVGE_state.logger, "Successfully initialized GLFW %s",
       glfwGetVersionString());
  AVGE_state.glfw_initialized = true;
  DONE(NL_OK);
  return AVGE_OK;
}
