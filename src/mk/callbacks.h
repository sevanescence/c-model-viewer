#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <stdio.h>

#include <GLFW/glfw3.h>

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
        GLint polygon_mode;
        glGetIntegerv(GL_POLYGON_MODE, &polygon_mode);
        switch (polygon_mode) {
        case GL_FILL:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case GL_LINE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        case GL_POINT:
        default:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

#endif