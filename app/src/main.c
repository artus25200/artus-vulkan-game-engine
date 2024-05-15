#include <avge.h>
#include <nicelog.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  BEGIN("Initializing APP");
  App *app = AVGE_create_application("TEST APP", AVGE_VERSION(1, 0, 0));
  if (app == NULL) {
    DONE(NL_ABORTED);
    return AVGE_EXIT_FAILURE;
  }
  DONE(NL_OK);

  if (!AVGE_initialize_engine(app)) {
    DONE(NL_ERROR);
    return AVGE_EXIT_FAILURE;
  }

  BEGIN("Creating Window");
  AVGE_create_window(app, 800, 600, app->name);
  DONE(NL_OK);

  return AVGE_EXIT_SUCCESS;
}
