#include <stdio.h>

#define ENGINE_C_
#include "avge.h"
#undef ENGINE_C_

#include <stdlib.h>

AVGEStatusCode AVGE_initialize() {
  AVGE_internal_logger = NL_create_logger("AVGE");
  return AVGE_OK;
}
