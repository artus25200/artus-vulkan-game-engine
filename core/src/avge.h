/*
**   Artus25200's Vulkan Game Engine (AVGE)
*/

#ifndef AVGE_H_
#define AVGE_H_
#include <nicelog.h>

typedef enum { AVGE_ERROR, AVGE_WARN, AVGE_OK } AVGEStatusCode;
typedef enum { AVGE_EXIT_SUCCESS, AVGE_EXIT_FAILURE } AVGEExitCode;

/***************************
** CORE
***************************/

// --- ENGINE --- //
AVGEStatusCode AVGE_initialize(void);

// --- APP --- //
typedef struct App App;
App *AVGE_create_application(char *name, char *version);
Logger *AVGE_get_app_logger(App *app);

/**************************
** VULKAN
**************************/

AVGEStatusCode AVGE_initialize_vulkan(void);

#endif // AVGE_H_
