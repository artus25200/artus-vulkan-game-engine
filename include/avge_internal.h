#ifndef INTERNAL_H_
#define INTERNAL_H_
#include <avge.h>
#include <stdbool.h>

struct EngineState {
  bool initialized;
  Logger *logger;
};

#ifdef ENGINE_C_
struct EngineState AVGE_state;
#else
extern struct EngineState AVGE_state;
#endif // ENGINE_C_

#endif // INTERNAL_H_
