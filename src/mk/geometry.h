// #ifndef GEOMETRY_H
// #define GEOMETRY_H

// #include <glad/glad.h>

// #include <cglm/cglm.h>

// #include "shader.h"
// #include "vec.h"

// typedef struct Vertex {
//     vec3            position;
//     vec2            tex_coords;
// } Vertex;

// typedef struct Texture {
//     GLuint          id;
// } Texture;

// typedef enum ImageType {
//     IMAGE_PNG =     1,
//     IMAGE_JPG =     2
// } ImageType;

// Texture texture_create_from_image(const char *image_path, ImageType image_type) {
//     // TODO: initialize image.

//     Texture texture;

//     return texture;
// }

// VEC_DECLARE(Vertex)
// VEC_DECLARE(uint32_t)
// // TODO: move definitions to header-safe file, preferrably main or some auxiliary source file.
// VEC_DEFINE(Vertex)
// VEC_DEFINE(uint32_t)


// /// List of vertices with single texture.
// typedef struct Mesh {
//     Vec(Vertex)     vertices;
//     Vec(uint32_t)   indices;
//     Texture         texture;

//     vec3            position;
//     mat4            rotation;

//     GLuint          m_VAO;
//     GLuint          m_VBO;
//     GLuint          m_EBO;
// } Mesh;

// #define MESH_INIT {}

// void mesh_init_from_vertices(Mesh *self, Vertex *vertices, size_t size) {
//     // waste no time nor space reallocating
//     VEC_DESTROY(Vertex, &self->vertices);
//     VEC_RESERVE(Vertex, &self->vertices, size);

//     // TODO: Implement push_all function for vec or something
//     memcpy(self->vertices.data, vertices, size * sizeof (Vertex));
//     self->vertices.size = size;
// }

// void mesh_draw(Mesh *self, Shader *shader) {
//     glUseProgram(shader->id);

//     if (self->texture.id) {
//         // TODO: Load image buffer
//     }

//     glDrawArrays(GL_TRIANGLES, 0, 36);
// }

// #endif