#include <avge.h>
#include <avge_internal.h>
#include <nicelog.h>
#include <stdlib.h>

struct App {
  char *name;
  char *version;
  Renderer *renderer;
  Logger *logger;
};

App *AVGE_create_application(char *name, char *version) {
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
  // return AVGE_create_renderer(&app->renderer);
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
