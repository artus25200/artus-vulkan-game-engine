#include "avge.h"
#include "nicelog/nicelog.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  BEGIN("Initializing");
  BEGIN("Initializing AVGE");

  if (!AVGE_initialize()) {
    DONE(NL_ERROR);
    return AVGE_ERROR;
  }
  DONE(NL_OK);

  BEGIN("Initializing APP");
  App *app = AVGE_create_application("TEST APP", "1.0.0");
  if (app == NULL) {
    DONE(NL_ERROR);
    return AVGE_ERROR;
  }
  Logger *app_logger = AVGE_get_app_logger(app);

  DONE(NL_OK);
  DONE(NL_OK);

  BEGIN("Running app");
  ERROR(app_logger, "Not implemented.");
  DONE(NL_SKIPPED);

  return EXIT_SUCCESS;
}
