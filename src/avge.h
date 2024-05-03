/*
**   Artus25200's Vulkan Game Engine (AVGE)
*/

#ifndef AVGE_H_
#define AVGE_H_
#include "nicelog.h"

typedef enum { AVGE_ERROR, AVGE_WARN, AVGE_OK } AVGEStatus;

typedef struct {

} Renderer;

typedef struct {
  char *name;
  char *version;
  Renderer renderer;
  Logger logger;
} App;

AVGEStatus AVGE_initialize_application(App *app, char *name, char *version);

#endif // AVGE_H_
