#ifndef MK_SHADER_H
#define MK_SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

typedef struct Shader {
    GLuint      id;
    GLint *     uniforms;
} Shader;

#define SHADER_INIT_FAILED(ID) (ID==GL_ZERO)
#define SHADER_COMPILATION_FAILED(RES) (!RES)
#define PROGRAM_LINK_FAILED(RES) (!RES)

#define CHECK_COMPILATION_FAILED(SHADER, MESSAGE) \
    { \
        int result; \
        char info_log[512]; \
        glGetShaderiv(SHADER, GL_COMPILE_STATUS, &result); \
        if (SHADER_COMPILATION_FAILED(result)) { \
            glGetShaderInfoLog(SHADER, 512, NULL, info_log); \
            fprintf(stderr, "%s: %511s", MESSAGE, info_log); \
            abort(); \
        } \
    } \

#define CHECK_SHADER_LINK_FAILED(PROGRAM, MESSAGE) \
    { \
        int result; \
        char info_log[512]; \
        glGetProgramiv(PROGRAM, GL_LINK_STATUS, &result); \
        if (PROGRAM_LINK_FAILED(result)) { \
            glGetProgramInfoLog(PROGRAM, 512, NULL, info_log); \
            fprintf(stderr, "%s: %511s", MESSAGE, info_log); \
            abort(); \
        } \
    }


Shader *shader_create_from_sources(const char *vertex_src, const char *fragment_src) {
    GLuint id = glCreateProgram();
    if (SHADER_INIT_FAILED(id)) {
        fprintf(stderr, "Failed to create shader program.");
        abort();
    }

    Shader *shader = malloc(sizeof *shader);
    shader->id = id;
    shader->uniforms = (GLint *)NULL;

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (SHADER_INIT_FAILED(vertex_shader) || SHADER_INIT_FAILED(fragment_shader)) {
        fprintf(stderr, "Failed to create shaders.");
        abort();
    }

    // -- Vertex shader building
    glShaderSource(vertex_shader, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader);
    CHECK_COMPILATION_FAILED(
        vertex_shader,
        "Failed to compile vertex shader");
    
    glShaderSource(fragment_shader, 1, &fragment_src, NULL);
    glCompileShader(fragment_shader);
    CHECK_COMPILATION_FAILED(
        fragment_shader,
        "Failed to compile fragment shader");
    
    glAttachShader(shader->id, vertex_shader);
    glAttachShader(shader->id, fragment_shader);
    glLinkProgram(shader->id);

    CHECK_SHADER_LINK_FAILED(shader->id, "Failed to link shader program");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // TODO: Create uniform array

    return shader;
}

void mk_shader_free(Shader *shader) {
    free(shader->uniforms);
    free(shader);
}

#endif