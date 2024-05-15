#include <avge.h>
#include <avge_internal.h>
#include <nicelog.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

App *AVGE_create_application(char *name, uint32_t version) {
  check_for_logger();

  App *app = malloc(sizeof(struct App));
  if (app == NULL) {
    ERROR(AVGE_state.logger, "Failed to malloc app %s", name);
    return NULL;
  }
  app->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(app->name, name);
  app->version = version;
  app->logger = NL_create_logger(name);
  INFO(AVGE_state.logger, "Successfully initialized app %s %d.%d.%d", name,
       VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version),
       VK_VERSION_PATCH(version));
  return app;
}

Logger *AVGE_get_app_logger(App *app) {
  check_for_logger();

  if (app == NULL) {
    WARN(AVGE_state.logger, "Could not get logger, app object is NULL");
    return NULL;
  }
  return app->logger;
}
