#include <avge.h>
#include <nicelog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENGINE_C_
#include <avge_internal.h>
#undef ENGINE_C_

void check_for_logger() {
  if (AVGE_state.logger != NULL)
    return;
  AVGE_state.logger = NL_create_logger("AVGE");
  if (AVGE_state.logger == NULL) {
    ERROR(NULL, "Failed to create AVGE Logger.");
  }
}

AVGEStatusCode AVGE_initialize_engine(App *app) {
  BEGIN("Initializing AVGE");
  check_for_logger();
  if (!AVGE_initialize_glfw() || !AVGE_initialize_vulkan(app)) {
    FATAL(AVGE_state.logger, "Could not initialize AVGE !");
    DONE(NL_ABORTED);
    return AVGE_ERROR;
  }
  AVGE_state.engine_initialized = true;
  INFO(AVGE_state.logger, "Successfully initialized AVGE");
  DONE(NL_OK);
  return AVGE_OK;
}

void AVGE_terminate_engine() {
  AVGE_print_memory_debug();
  AVGE_free_all();
  AVGE_print_memory_debug();
}
