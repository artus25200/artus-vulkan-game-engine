/*
**   Artus25200's Vulkan Game Engine (AVGE)
*/

#ifndef AVGE_H_
#define AVGE_H_
#include "nicelog/nicelog.h"

typedef enum { AVGE_ERROR, AVGE_WARN, AVGE_OK } AVGEStatusCode;

typedef struct Renderer Renderer;

/***************************
** ENGINE
***************************/

#ifdef ENGINE_C_
Logger *AVGE_internal_logger;
#else
extern Logger *AVGE_internal_logger;
#endif // ENGINE_C_

AVGEStatusCode AVGE_initialize(void);

/***************************
** APP
***************************/

typedef struct App App;
App *AVGE_create_application(char *name, char *version);
Logger *AVGE_get_app_logger(App *app);
#endif // AVGE_H_
