#pragma once
#include "entityComponentManager.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace components {

    struct VertexComponent {
        std::vector<float> vertices;
    };

    struct IndexComponent {
        std::vector<unsigned int> indices;
    };

    struct TextureComponent {
        unsigned int textureID;
        std::string textureName;
    };

    struct ShaderComponent {
        std::string shaderName;

        ShaderComponent(const std::string& name = "") : shaderName(name) {}
    };
    

    struct ModelMatrixComponent {
        glm::mat4 model = glm::mat4(1.0f);  // Initialize to identity matrix
    };

    struct CameraComponent {
        glm::vec3 position{ 0.0f, 0.0f, 3.0f };
        glm::vec3 front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 up{ 0.0f, 1.0f, 0.0f };
        float yaw{ -90.0f };
        float pitch{ 0.0f };
        float movementSpeed{ 2.5f };
        float mouseSensitivity{ 0.1f };
        float fov{ 45.0f };
        float aspectRatio;
        float nearPlane{ 0.1f };
        float farPlane{ 100.0f };

        glm::mat4 viewMatrix() const {
            return glm::lookAt(position, position + front, up);
        }

        glm::mat4 projectionMatrix() const {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }
    };

    struct windowSettingsComponent  {
        std::string windowIdentifier = "defaultIdentifier";
        const char* windowTitle = "defaultWindowTitle";
        float windowHeight = 600;
        float windowWidth = 800;
    };

    struct projectionComponent {
        glm::mat4 view;
        glm::mat4 projection;
        
    };

    struct deltaTimeComponent {
        float deltaTime = 0.0f;
        float lastFrameTime = 0.0f;
    };

}//component namespace