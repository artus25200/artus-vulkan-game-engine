#include "avge.h"
#include "nicelog/nicelog.h"

int main(void) {
  INFO("Starting app...");
  App app;
  BEGIN("Initializing app");

  app.name = "ENGINE TEST APP";
  app.version = "0.1.0";
  NL_set_app_name(app.name);
  INFO("Initialized app %s : %s", app.name, app.version);
  DONE(NL_OK);

  BEGIN("Running app");
  ERROR("Not implemented.");
  DONE(NL_SKIPPED);

  return AVGE_OK;
}
