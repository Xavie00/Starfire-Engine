#pragma once
#include <vector>
#include "components.h"
#include "entityComponentManager.h"
struct Cube {
    std::vector<float> vertices = {
        // Positions          // Colors            // Texture Coords
 // Front face
 -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right
  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // Top-right
 -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top-left

 // Back face
 -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Bottom-left
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,  // Bottom-right
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f,  // Top-right
 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // Top-left

 // Left face
 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // Top-right
 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Top-left
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // Bottom-left
 -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-right

 // Right face
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // Top-left
  0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
  0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // Bottom-right
  0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left

  // Top face
  -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // Top-left
   0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // Bottom-right
  -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left

  // Bottom face
  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // Top-right
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Top-left
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // Bottom-left
  -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f   // Bottom-right
    };
    
    std::vector<unsigned int> indicies = { 
        // Front face
        0, 1, 2,
        2, 3, 0,

        // Back face
        4, 5, 6,
        6, 7, 4,

        // Left face
        8, 9, 10,
        10, 11, 8,

        // Right face
        12, 13, 14,
        14, 15, 12,

        // Top face
        16, 17, 18,
        18, 19, 16,

        // Bottom face
        20, 21, 22,
        22, 23, 20
    };
};

void assignShapeToEntity(int entity, ECS::EntityComponent& ecs, std::vector<float>& vertices, std::vector<unsigned int>& indicies) {
    // Add position, color, texture coordinate, and texture components to the entity
    ecs.addComponent(entity, components::VertexComponent{ vertices });
    ecs.addComponent(entity, components::IndexComponent{ indicies });
    ecs.addComponent(entity, components::ModelMatrixComponent{});
}