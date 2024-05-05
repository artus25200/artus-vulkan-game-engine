#include <avge.h>
#include <nicelog.h>
#include <stdio.h>
#include <stdlib.h>

#define ENGINE_C_
#include <avge_internal.h>
#undef ENGINE_C_

AVGEStatusCode AVGE_initialize() {
  AVGE_state.logger = NL_create_logger("AVGE");
  if (AVGE_state.logger == NULL) {
    ERROR(NULL, "Failed to create internal AVGE logger");
    return AVGE_ERROR;
  }
  AVGE_state.initialized = true;
  INFO(AVGE_state.logger, "Successfully initialized AVGE");
  return AVGE_OK;
}
