#include <GLFW/glfw3.h>
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
    return AVGE_EXIT_FAILURE;
  }

  BEGIN("Creating Window");
  AVGE_create_window(app, 800, 600, app->name);
  DONE(NL_OK);

  /*while (!glfwWindowShouldClose(app->glfw_window)) {
    glfwPollEvents();
    glfwSwapBuffers(app->glfw_window);
  }*/

  AVGE_terminate_engine();
  return AVGE_EXIT_SUCCESS;
}
