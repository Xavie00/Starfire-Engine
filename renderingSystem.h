#pragma once
#include <glad/glad.h>
#include "ShaderManager.h"
#include "TextureManager.h"
#include "cachedECS.h"
#include "components.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "window.h"
#include "cameraManager.h"



void renderEntities(ECS::EntityComponent& ecs, ShaderManager& shaderManager, TextureManager& textureManager) {
    std::vector<ECS::EntityID> entitiesToRender;

    // Gather entities that have the required components
    for (int entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
        if (ecs.hasComponents<components::VertexComponent, components::IndexComponent, components::ShaderComponent, components::TextureComponent>(entity)) {
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

        auto shader = shaderManager.getShader(shaderComponent->shaderName);

        glUseProgram(shader);

        // Set the model matrix uniform
        unsigned int modelLoc = glGetUniformLocation(shader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrixComponent->model));

        // Set the view and projection matrices (assuming they are constant for all entities)
        unsigned int viewLoc = glGetUniformLocation(shader, "view");
        unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        if (vertexComponent && indexComponent && shaderComponent && textureComponent) {
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

            // Bind the texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureManager.getTexture(textureComponent->textureName));

            // Draw the entity
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexComponent->indices.size()), GL_UNSIGNED_INT, 0);
        }
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

