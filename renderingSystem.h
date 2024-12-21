#pragma once
#include <glad/glad.h>
#include "ShaderManager.h"
#include "TextureManager.h"
#include "entityComponentManager.h"
#include "components.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cameraManager.h"

void renderEntities(ECS::EntityComponent& ecs, ShaderManager& shaderManager, TextureManager& textureManager, glm::mat4 view, glm::mat4 projection) {
    std::vector<ECS::EntityID> entitiesToRender;

    // Gather entities with different sets of components
    for (int entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
        // Check if the entity has at least Vertex and Index components
        if (ecs.hasComponents<components::VertexComponent, components::IndexComponent>(entity)) {
            entitiesToRender.push_back(entity);
        }
    }

    // Create buffer objects outside the loop
    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    for (const auto& entity : entitiesToRender) {
        auto* vertexComponent = ecs.getComponent<components::VertexComponent>(entity);
        auto* indexComponent = ecs.getComponent<components::IndexComponent>(entity);
        auto* shaderComponent = ecs.getComponent<components::ShaderComponent>(entity);
        auto* textureComponent = ecs.getComponent<components::TextureComponent>(entity);
        auto* modelMatrixComponent = ecs.getComponent<components::ModelMatrixComponent>(entity);

        // Use default shader if entity doesn't have its own shader
        unsigned int shader;
        if (shaderComponent) {
            shader = shaderManager.getShader(shaderComponent->shaderName);
        }
        else {
            shader = shaderManager.getShader("default"); // Use a default shader if no shader is attached
        }

        glUseProgram(shader);

        // Set the model matrix uniform
        if (modelMatrixComponent) {
            unsigned int modelLoc = glGetUniformLocation(shader, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrixComponent->model));
        }

        // Set the view and projection matrices
        unsigned int viewLoc = glGetUniformLocation(shader, "view");
        unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        if (vertexComponent && indexComponent) {
            // Update the VBO and IBO data
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER,
                vertexComponent->vertices.size() * sizeof(float),  // Size of the data in bytes
                vertexComponent->vertices.data(),                  // Pointer to the data
                GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                indexComponent->indices.size() * sizeof(unsigned int),  // Size of the data in bytes
                indexComponent->indices.data(),                         // Pointer to the data
                GL_STATIC_DRAW);

            // Set vertex attribute pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // Bind the texture if the entity has a texture
            if (textureComponent) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureManager.getTexture(textureComponent->textureName));
            }
            
            // Draw the entity
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexComponent->indices.size()), GL_UNSIGNED_INT, 0);
        }
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

unsigned int createNullTexture() {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Create a 1x1 pixel texture with a solid color (e.g., white)
    unsigned char whitePixel[3] = { 255, 255, 255 }; // RGB values for white
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whitePixel);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
    return textureID;
}