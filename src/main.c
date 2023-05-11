#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <direct.h>

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
#include "mk/resources.h"
#include "mk/callbacks.h"
#include "mk/shader.h"

#include "mk/logging.h"

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
    model_init(&model, "./resources/horse/scene.gltf");
    for (unsigned int i = 0; i < model.meshes.size; ++i) {
        mesh_m_configure_gl_data(&model.meshes.data[i]);
    }

    // Model EARTH = {};
    // model_init(&EARTH, "./sludge/earth.glb");
    // for (unsigned int i = 0; i < EARTH.meshes.size; ++i) {
    //     mesh_m_configure_gl_data(&EARTH.meshes.data[i]);
    // }
    // GLuint earth_texture;
    // {

    // }

    /// TODO: Load this elsewhere.
    Shader *default_shader = NULL;
    {
        Resource vertex_src = mk_get_resource("vertex.glsl");
        Resource fragment_src = mk_get_resource("fragment.glsl");

        default_shader = shader_create_from_sources(vertex_src.data, fragment_src.data);

        mk_resource_destruct(&vertex_src);
        mk_resource_destruct(&fragment_src);
    }

    GLuint texture;
    // {
    //     glGenTextures(1, &texture);
    //     glBindTexture(GL_TEXTURE_2D, texture);

    //     // WE LOVE COPYING AND PASTING CODE
    //     // set the texture wrapping/filtering options (on the currently bound texture object)
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     // load and generate the texture
    //     int width, height, nrChannels;
        
    //     unsigned char *data = stbi_load("./sludge/terrezed_atlas.png", &width, &height, &nrChannels, 0);
    //     if (data) {
    //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //         glGenerateMipmap(GL_TEXTURE_2D);
            
    //     } else {
    //         fprintf(stderr, "Image read failure.\n");
    //         fflush(stderr);
    //         abort();
    //     }

    //     stbi_image_free(data);
        
    // }





















    // TODO: Just get UI working and import models with external atlasses!
    


























    
    while (!glfwWindowShouldClose(context)) {
        glClearColor(0.3, 0.3, 0.35, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #define radians(deg) (deg*(M_PI/180))

        int width, height;
        glfwGetWindowSize(context, &width, &height);

        mat4 projection = GLM_MAT4_IDENTITY;
        glm_perspective(radians(60.0f), ((float)width)/height, 0.1f, 100000.0f, projection);
        //glm_ortho(0.0f, 32.0f,0.0f, 32.0f, 0.1f, 100.0f, projection);

        // temporary controls for projection and model testing. TODO: Replace with camera control manager
        static float z = -2.0f;
        static float y = -1.0f;
        static float x = 0.0f;
        const float mult = 0.0005;
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

        // // Terrezed BIG

        // // mat4 model_mat = GLM_MAT4_IDENTITY;
        // // glm_translate(model_mat, (vec3){ x, y, z });
        // // glm_rotate(model_mat, radians(-90), (vec3){ 1, 0, 0 });
        // // double m = glfwGetTime() / 1000 * 600 + 1;
        // // glm_scale(model_mat, (vec3){ m, m, m });

        // // glm_mul(projection, model_mat, projection);

        // // glUseProgram(default_shader->id);
        // // glUniformMatrix4fv(glGetUniformLocation(default_shader->id, "projection"), 1, GL_FALSE, projection[0]);

        // // model_draw(&model, default_shader);

        // Terrezed

        mat4 model_mat = GLM_MAT4_IDENTITY;
        glm_mat4_identity(model_mat);
        glm_translate(model_mat, (vec3){ x, y, z });
        //glm_rotate(model_mat, radians(-90), (vec3){ 1, 0, 0 });
        glm_rotate(model_mat, radians(-90), (vec3){ 0, 1, 0 });
        glm_scale(model_mat, (vec3){ 0.02, 0.02, 0.02 });

        glm_perspective(radians(60.0f), ((float)width)/height, 0.01f, 10000.0f, projection);
        glm_mul(projection, model_mat, projection);

        glUseProgram(default_shader->id);
        glUniformMatrix4fv(glGetUniformLocation(default_shader->id, "projection"), 1, GL_FALSE, projection[0]);
        
        model_draw(&model, default_shader);

        // // EARTH

        // // glm_mat4_identity(model_mat);
        // // glm_translate(model_mat, (vec3){ x, y, z - 1200 * 5 });
        // // glm_scale(model_mat, (vec3){ 5, 5, 5 });

        // // glm_perspective(radians(60.0f), ((float)width)/height, 0.1f, 100000.0f, projection);
        // // glm_mul(projection, model_mat, projection);

        // // glUseProgram(default_shader->id);
        // // glUniformMatrix4fv(glGetUniformLocation(default_shader->id, "projection"), 1, GL_FALSE, projection[0]);
        
        // // model_draw(&EARTH, default_shader);
        
        glfwPollEvents();
        glfwSwapBuffers(context);
    }

    mk_shader_free(default_shader);
    glfwTerminate();
}
