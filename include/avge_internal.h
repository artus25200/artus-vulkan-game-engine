#ifndef INTERNAL_H_
#define INTERNAL_H_
#include <avge.h>
#include <nicelog.h>
#include <stdbool.h>

struct EngineState {
  bool engine_initialized;
  bool vulkan_initialized;
  bool glfw_initialized;
  Logger *logger;
};

#ifdef ENGINE_C_
struct EngineState AVGE_state = {.logger = NULL};
#else
extern struct EngineState AVGE_state;
#endif // ENGINE_C_

void check_for_logger();

#endif // INTERNAL_H_
