#include <avge.h>
#include <nicelog.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  BEGIN("Initializing AVGE");

  if (!AVGE_initialize()) {
    DONE(NL_ERROR);
    return AVGE_EXIT_FAILURE;
  }
  DONE(NL_OK);

  BEGIN("Initializing APP");
  App *app = AVGE_create_application("TEST APP", AVGE_VERSION(1, 0, 0));
  if (app == NULL) {
    DONE(NL_ABORTED);
    return AVGE_EXIT_FAILURE;
  }
  Logger *app_logger = AVGE_get_app_logger(app);

  DONE(NL_OK); // Initializing App

  BEGIN("Running app");
  ERROR(app_logger, "Not implemented.");
  DONE(NL_SKIPPED);

  return AVGE_EXIT_SUCCESS;
}
