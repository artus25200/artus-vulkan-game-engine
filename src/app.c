#include "avge.h"
#include "nicelog/nicelog.h"
#include <stdlib.h>

struct App {
  char *name;
  char *version;
  Renderer *renderer;
  Logger *logger;
};

App *AVGE_create_application(char *name, char *version) {
  App *app = malloc(sizeof(struct App));
  if (app == NULL) {
    ERROR(AVGE_internal_logger, "Failed to malloc app %s", name);
    return NULL;
  }
  app->name = name;
  app->version = version;
  app->logger = NL_create_logger(name);
  INFO(AVGE_internal_logger, "Successfully initialized app \"%s\" %s", name,
       version);
  // return AVGE_create_renderer(&app->renderer);
  return app;
}

Logger *AVGE_get_app_logger(App *app) {
  if (app == NULL) {
    WARN(AVGE_internal_logger, "Could not get logger, app object is NULL");
    return NULL;
  }
  return app->logger;
}
