#include<iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <shader/shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "camera.h"
#include "../external_lib/shader.h"

using namespace glm;


bool loadOBJ(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs,
             std::vector<glm::vec3> &out_normals);

class Object {
public:
    Shader shadersPtr = Shader(nullptr, nullptr);

    std::vector<vec3> position;
    std::vector<vec2> texture;
    std::vector<vec3> normal;

    GLuint VAO;
    GLuint VBO;
    GLuint VBONormal;

    mat4 Model;

    Object(mat4 model) {
        Model = model;
    }

    Object() {}

    bool loadObj(const char *path) {
        return loadOBJ(path, position, texture, normal);
    }

//    void bind_VAO_VBO(const char *pathTexture) {
    void bind_VAO_VBO() {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(vec3), &position[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);

        GLuint VBONormal;
        glGenBuffers(1, &VBONormal);
        glBindBuffer(GL_ARRAY_BUFFER, VBONormal);
        glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(vec3), &normal[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void loadSahders(const char *vertexPath, const char *fragmentPath) {
        shadersPtr.~Shader();
        new(&shadersPtr) Shader(vertexPath, fragmentPath);
    }

    void setObjectColor(float r, float g, float b) {
        shadersPtr.use();
        shadersPtr.setVec3("objectColor", r, g, b);
    }

    void setLightColor(float r, float g, float b) {
        shadersPtr.use();
        shadersPtr.setVec3("lightColor", r, g, b);
    }
//    --------------------------------------------------------
    void updateLightPos(vec3 lightPos){
        shadersPtr.use();
        shadersPtr.setVec3("lightPos", lightPos);
    }
    void updateViewPos(Camera camera){
        shadersPtr.use();
        shadersPtr.setVec3("viewPos", camera.Position);
    }
//   ------------------------------------------------------------
    void drawObject(Camera camera) {
        shadersPtr.use();
        shadersPtr.setMat4("projection", camera.UpdatedProjection());
        shadersPtr.setMat4("view", camera.UpdatedView());
        shadersPtr.setMat4("model", Model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, position.size());
    }

    void delete_VAO_Buffers() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &VBONormal);
    };


};

class LightObj: public Object{
public:
    vec3 LightPos;
    LightObj(mat4 model, vec3 lightPos){
        Model = translate(mat4(1.0f), lightPos);
        LightPos= lightPos;
    };
};


bool loadOBJ(
        const char *path,
        std::vector<glm::vec3> &out_vertices,
        std::vector<glm::vec2> &out_uvs,
        std::vector<glm::vec3> &out_normals) {
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file !\n");
        getchar();
        return false;
    }

    while (1) {

        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y;
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],
                                 &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read \n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        } else {
            // Probably a comment
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    std::cout << vertexIndices.size() << std::endl;

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        glm::vec2 uv = temp_uvs[uvIndex - 1];
        glm::vec3 normal = temp_normals[normalIndex - 1];

        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(normal);
    }

    return true;
}
