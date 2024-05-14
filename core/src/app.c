#include <avge.h>
#include <avge_internal.h>
#include <nicelog.h>
#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

App *AVGE_create_application(char *name, uint32_t version) {
  if (!AVGE_state.initialized) {
    ERROR(NULL, "AVGE not initialized.");
    return NULL;
  }
  App *app = malloc(sizeof(struct App));
  if (app == NULL) {
    ERROR(AVGE_state.logger, "Failed to malloc app %s", name);
    return NULL;
  }
  app->name = name;
  app->version = version;
  app->logger = NL_create_logger(name);
  INFO(AVGE_state.logger, "Successfully initialized app \"%s\" %s", name,
       version);
  return app;
}

Logger *AVGE_get_app_logger(App *app) {
  if (!AVGE_state.initialized) {
    ERROR(NULL, "AVGE not initialized.");
    return NULL;
  }
  if (app == NULL) {
    WARN(AVGE_state.logger, "Could not get logger, app object is NULL");
    return NULL;
  }
  return app->logger;
}
