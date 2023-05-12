#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include <glad/glad.h>
#include <cglm/cglm.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <string.h>

#include "logging.h"
#include "shader.h"
#include "vec.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

typedef struct ModelVertex {
    vec3                position;
    vec3                normal;
    vec2                tex_coords;
} ModelVertex;

typedef enum TextureType {
    TEXTURE_DIFFUSE =   1,
    TEXTURE_SPECULAR =  2
} TextureType;

const char *texture_type_to_string(TextureType type) {
    switch (type) {
    case TEXTURE_DIFFUSE:
        return "diffuse";
    case TEXTURE_SPECULAR:
        return "specular";
    default:
        return "";
        break;
    }
}

typedef struct Texture {
    GLuint              id;
    TextureType         type;
} Texture;

VEC_DECLARE(ModelVertex)
VEC_DEFINE(ModelVertex)

VEC_DECLARE(uint32_t)
VEC_DEFINE(uint32_t)

VEC_DECLARE(Texture)
VEC_DEFINE(Texture)

typedef struct Mesh {
    Vec(ModelVertex)    vertices;
    Vec(uint32_t)       indices;
    GLuint              texture_diffuse;

    GLuint              VAO;
    GLuint              VBO;
    GLuint              EBO;
} Mesh;

void mesh_m_configure_gl_data(Mesh *mesh) {
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size * sizeof (ModelVertex), mesh->vertices.data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size * sizeof (uint32_t), mesh->indices.data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (ModelVertex), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (ModelVertex), (void *)offsetof(ModelVertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (ModelVertex), (void *)offsetof(ModelVertex, tex_coords));

    glBindVertexArray(0);
}

void mesh_init(Mesh *mesh, Vec(ModelVertex) vertices, Vec(uint32_t) indices) {
    mesh->vertices = vertices;
    mesh->indices = indices;

    mesh_m_configure_gl_data(mesh);
}

void mesh_destroy(Mesh *mesh) {
    VEC_DESTROY(ModelVertex, &mesh->vertices);
    VEC_DESTROY(uint32_t, &mesh->indices);
}

// NOTE: Read Rendering section for the specular map stuff
// Draws mesh to the screen. Assumes the passed shader is the corresponding mesh shader.
// TODO: Make shader member of mesh.
void mesh_draw(Mesh *mesh, Shader *shader) {
    glUseProgram(shader->id);

    // GLuint diffuse_count = 1;
    // GLuint specular_count = 1;
    // for (GLuint i = 0; i < mesh->textures.size; ++i) {
    //     glActiveTexture(GL_TEXTURE0 + i);

    //     char number[4]; // this isnt necessary since im formatting a number in-string anyway.
    //     TextureType type = mesh->textures.data[i].type;
    //     switch (type) {
    //     case TEXTURE_DIFFUSE:
    //         sprintf(number, "%d", diffuse_count);
    //         break;
    //     case TEXTURE_SPECULAR:
    //         sprintf(number, "%d", specular_count);
    //         break;
    //     default:
    //         fprintf(stderr, "Unexpected result in TextureType match.");
    //         fflush(stderr);
    //         abort();
    //         break;
    //     }
        
    //     char uniform_name[32];
    //     sprintf(uniform_name, "texture_%s%s", texture_type_to_string(type), number);
    //     glUniform1i(glGetUniformLocation(shader->id, uniform_name), i);
    //     glBindTexture(GL_TEXTURE_2D, mesh->textures.data[i].id);
    // }

    glBindTexture(GL_TEXTURE_2D, mesh->texture_diffuse);
    //mklog_fmt("%d\n", mesh->texture_diffuse);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indices.size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

VEC_DECLARE(Mesh)
VEC_DEFINE(Mesh)

typedef struct Model {
    Vec(Mesh)           meshes;
    char *              path;
} Model;



void model_draw(Model *model, Shader *shader) {
    for (size_t i = 0; i < model->meshes.size; ++i) {
        mesh_draw(&model->meshes.data[i], shader);
    }
}

// TODO: Implement this ~~yeah~~
GLuint texture_from_file(const char *path, const char *dir) {
        // load obama.png
    // int width, height, channels;
    // unsigned char *obama_image = stbi_load("./resources/obama.png", &width, &height, &channels, 0);
    // if (!obama_image) {
    //     fprintf(stderr, "Failed to load image.");
    //     abort();
    // }
    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, obama_image);
    // glGenerateMipmap(GL_TEXTURE_2D);
    // stbi_image_free(obama_image);
    // TODO: Write path module
    char resource_location[1024];
    sprintf("%s/%s", dir, path);
    int width, height, channel_count;
    unsigned char *image = stbi_load(resource_location, &width, &height, &channel_count, 0);

    

    return 0;
}

Vec(Texture) model_m_load_material_textures(Model *model, struct aiMaterial *mat, enum aiTextureType type, TextureType texture_type) {
    Vec(Texture) vec;

    for (unsigned int i = 0; i < aiGetMaterialTextureCount(mat, type); ++i) {
        struct aiString str;
        aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
        Texture texture;
        texture.id = texture_from_file(str.data, model->path);
        texture.type = texture_type;
        // TODO: texture.path ?
        VEC_PUSH_BACK(Texture, &vec, texture);
    }

    return vec;
}

/// NOTE: This section was very rushed due to finals. I'd like to rework the algorithm when I have the time.
Mesh model_m_process_mesh(Model *model, struct aiMesh *ai_mesh, const struct aiScene *scene) {
    Mesh mesh = {};

    Vec(ModelVertex) vertices = VEC_INIT;
    Vec(uint32_t) indices = VEC_INIT;

    mklog("processing vertices...\n");
    mklog_fmt("Vertices: %d\n", ai_mesh->mNumVertices);
    mklog_fmt("Indices: %d\n", ai_mesh->mNumFaces);
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
        ModelVertex vertex;
        // TODO: process vertex positions, normals, and texture coordinates.
        vertex.position[0] = ai_mesh->mVertices[i].x;
        vertex.position[1] = ai_mesh->mVertices[i].y;
        vertex.position[2] = ai_mesh->mVertices[i].z;
        vertex.normal[0] = ai_mesh->mNormals[i].x;
        vertex.normal[1] = ai_mesh->mNormals[i].y;
        vertex.normal[2] = ai_mesh->mNormals[i].z;
        if (ai_mesh->mTextureCoords[0]) {
            vertex.tex_coords[0] = ai_mesh->mTextureCoords[0][i].x;
            vertex.tex_coords[1] = ai_mesh->mTextureCoords[0][i].y;
        }
        VEC_PUSH_BACK(ModelVertex, &vertices, vertex);
    }
    mklog("Processing indices...\n")
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
        struct aiFace face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            VEC_PUSH_BACK(uint32_t, &indices, face.mIndices[j]);
        }
    }
    mklog("Polygons processed.\n")

    /// TODO: This is probably leaking VRAM because different mesh nodes can use the same texture.
    /// Load it before loading the meshes instead. Or don't, this is going in the trash anyway.
    if (ai_mesh->mMaterialIndex >= 0) {
        struct aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];

        // struct aiString name;
        // aiGetMaterialString(material, AI_MATKEY_NAME, &name);
        // mklog_fmt("%s\n", name.data);

        /// TODO: Add support for more texture types. Only diffuse will work for now.
        /// NOTE: For the time being, texture locations are relative, even though some formats list
        /// them as absolute. This will be changed.
        struct aiString diffuse_path;
        aiGetMaterialTexture(material,
            aiTextureType_DIFFUSE,
            ai_mesh->mMaterialIndex,
           &diffuse_path,
        0, 0, 0, 0, 0, 0);

        mklog_fmt("Texture path: %s\n", diffuse_path.data);

        if (diffuse_path.data[0] == '*') {
            int ti = atoi(diffuse_path.data + 1);
            mklog_fmt("Texture index: %d\n", ti);

            mklog_fmt("%d %d\n", scene->mTextures[ti]->mWidth, scene->mTextures[ti]->mHeight);

            /// TODO: Isolate in another function
            GLuint texture_diffuse;
            glGenTextures(1, &texture_diffuse);
            
            glBindTexture(GL_TEXTURE_2D, texture_diffuse);

            // We will use linear interpolation for magnification filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            unsigned char *image_data = NULL; int width; int height; int nr_channels;
            {
                image_data = stbi_load_from_memory((unsigned char *)scene->mTextures[ti]->pcData, 
                    scene->mTextures[ti]->mWidth,
                    &width, &height, &nr_channels, 0);
            }

            // Texture specification
            if (nr_channels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
							image_data);
            } else if (nr_channels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
							image_data);
            } else {
                fprintf(stderr, "Unsupported channel count.");
                fflush(stderr);
                abort();
            }
            
            stbi_image_free(image_data);
            
            //glGenerateMipmap(GL_TEXTURE_2D);

            mesh.texture_diffuse = texture_diffuse;
        } else {
            /// NOTE: Using relative directory for now
            // const char *p = diffuse_path.data + diffuse_path.length - 1;
            // while (*p != '\\' || *p != '/') {
            //     --p;
            // }
            // mklog_fmt("%s\n", p);
            mklog_fmt("%d %s\n", diffuse_path.length, diffuse_path.data);
            mklog_fmt("%s\n", model->path);

            char *image_path = calloc(1, strlen(model->path) + diffuse_path.length + 1);
            memcpy(image_path, model->path, strlen(model->path));
            memcpy(image_path + strlen(model->path), diffuse_path.data, diffuse_path.length);

            mklog_fmt("%s\n", image_path);
            // const char *p = diffuse_path.data + diffuse_path.length - 1;
            // while (*p != '\\') {
            //     --p;
            // } ++p;
            // mklog_fmt("%s\n", p);

            /// NOTE: Hardcoded for now. TODO: CHANGE THIS! (I am probably just going to refactor everything)
            // char *tmp = calloc(1, sizeof "./resources/sludge/" + strlen(p) + 1);
            // memcpy(tmp, "./resources/sludge/", sizeof "./resources/sludge/");
            // memcpy(tmp + sizeof "./resources/sludge/" - 1, p, strlen(p));
            // mklog_fmt("%s\n", tmp);

            GLuint texture_diffuse;
            glGenTextures(1, &texture_diffuse);
            glBindTexture(GL_TEXTURE_2D, texture_diffuse);

            // WE LOVE COPYING AND PASTING CODE
            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load and generate the texture
            int width, height, nrChannels;
            
            unsigned char *data = stbi_load(image_path, &width, &height, &nrChannels, 0);
            if (data) {
                mklog_fmt("%d %d %d\n", width, height, nrChannels);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                mklog("Test\n");
                
            } else {
                fprintf(stderr, "Image read failure.\n");
                fflush(stderr);
                abort();
            }

            stbi_image_free(data);
            free(image_path);
            mesh.texture_diffuse = texture_diffuse;
        }
    }
    
    mesh.vertices = vertices;
    mesh.indices = indices;

    mklog("Returning mesh\n")
    return mesh;
}

void model_m_process_node(Model *model, struct aiNode *node, const struct aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        printf("Fetched aiMesh\n"); fflush(stdout);
        VEC_PUSH_BACK(Mesh, &model->meshes, model_m_process_mesh(model, mesh, scene));
        mklog("model processed")
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        model_m_process_node(model, node->mChildren[i], scene);
    }
}

void model_m_load_model(Model *model, const char *path) {
    const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_SortByPType);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        fflush(stdout);
        abort();
    }
    //model->path = path;

    printf("Processing root node...\n");
    fflush(stdout);
    model_m_process_node(model, scene->mRootNode, scene);
}

/// TODO: Abstract path semantic from model. Isolate since it is only for preprocessing.
void model_init(Model *model, const char *path) {
    char *s = calloc(1, strlen(path) + 1);
    
    // Cursor to last forwardslash to fetch directory
    const char *cursor = path + strlen(path);
    while (*cursor != '/') {
        --cursor;
    } ++ cursor;
    memcpy(s, path, cursor - path);
    mklog_fmt("Path directory: %s\n", s);

    model->path = s;
    model_m_load_model(model, path);
    printf("Model %s loaded.\n", path);
    fflush(stdout);

    free(model->path);
    model->path = NULL;
}

#endif