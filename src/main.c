#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mk/model_geometry.h"
#include "mk/callbacks.h"
#include "mk/shader.h"

/// --------------- FUTURE PLANS ---------------

/// - assimp support for dynamic model loading

/// --------------- ~~~~~~~~~~~~ ---------------

// TODO: Octree structure for voxel sculpting (and other primitives)
// TODO: Proper logging system (program typically terminates before streams can flush...)
// TODO: Specular/diffuse/normal maps for voxels, etc.

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(void) {
    // GLFW initialization
    if (!glfwInit()) {
        fprintf(stderr, "Could not initialize GLFW.\n");
        return EXIT_FAILURE;
    }
    printf("GLFW initialized.\n");
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *context = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Model Viewer", NULL, NULL);
    if (!context) {
        fprintf(stderr, "Could not create GLFW window context.\n");
        return EXIT_FAILURE;
    }
    printf("GLFW context initialized. Loading...\n");
    glfwMakeContextCurrent(context);
    glfwSetKeyCallback(context, key_callback);
    glfwSetFramebufferSizeCallback(context, framebuffer_size_callback);

    // GLAD initialization
    printf("Events loaded. Loading GLAD...\n");
    if (!gladLoadGLLoader((void *(*)(const char *))glfwGetProcAddress)) {
        fprintf(stderr, "Could not initialize GLAD.\n");
        return EXIT_FAILURE;
    }
    printf("Loaded OpenGL %s with GLAD.\n", glGetString(GL_VERSION));
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    fflush(stdout);

    // Z-buffer and antialiasing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // Disable VSync
    glfwSwapInterval(0);

    // STBI configurations
    stbi_set_flip_vertically_on_load(true);



    Model model = {};
    model_init(&model, "./teepee/teepee.glb");
    printf("%d\n", 69);
    fflush(stdout);
    printf("%d\n", model.meshes.data[0].VAO);
    for (unsigned int i = 0; i < model.meshes.size; ++i) {
        mesh_m_configure_gl_data(&model.meshes.data[i]);
    }
    fflush(stdout);



    // TODO: Move to resources folder, write resources asset manager
    const char *vertex_src = ""
        "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNor;\n"
        "layout (location = 2) in vec2 aTex;\n"
        "out vec4 color;\n"
        "uniform mat4 projection;\n"
        "out vec2 TexCoord;\n"
        "void main() { color = vec4(aTex.xyx, 1.0f); TexCoord = aTex; gl_Position = projection * vec4(aPos.xyz, 1.0f); }";
    const char *fragment_src = ""
        "#version 460 core\n"
        "out vec4 FragColor;\n"
        "in vec4 color;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D o_texture;\n"
        "void main() { FragColor = vec4(TexCoord.xy, 0.0f, 1.0f); }";
    Shader *default_shader = shader_create_from_sources(vertex_src, fragment_src);
    
    while (!glfwWindowShouldClose(context)) {
        glClearColor(0.5, 0.3, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #define radians(deg) (deg*(M_PI/180))

        int width, height;
        glfwGetWindowSize(context, &width, &height);

        mat4 projection = GLM_MAT4_IDENTITY;
        glm_perspective(radians(60.0f), ((float)width)/height, 0.1f, 1000.0f, projection);
        //glm_ortho(0.0f, 32.0f,0.0f, 32.0f, 0.1f, 100.0f, projection);

        // temporary controls for projection and model testing. TODO: Replace with camera control manager
        static float z = 0.0f;
        static float y = 0.0f;
        static float x = 0.0f;
        const float mult = 0.005f;
        if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS) {
            z += mult;
        }
        if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS) {
            z -= mult;
        }
        if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS) {
            x += mult;
        }
        if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS) {
            x -= mult;
        }
        if (glfwGetKey(context, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            y += mult;
        }
        if (glfwGetKey(context, GLFW_KEY_SPACE) == GLFW_PRESS) {
            y -= mult;
        }

        // end of temporary controls

        mat4 model_mat = GLM_MAT4_IDENTITY;
        glm_translate(model_mat, (vec3){ x, y, z });

        glm_mul(projection, model_mat, projection);

        glUseProgram(default_shader->id);
        glUniformMatrix4fv(glGetUniformLocation(default_shader->id, "projection"), 1, GL_FALSE, projection[0]);

        model_draw(&model, default_shader);
        
        glfwPollEvents();
        glfwSwapBuffers(context);
    }

    mk_shader_free(default_shader);
    glfwTerminate();
}
