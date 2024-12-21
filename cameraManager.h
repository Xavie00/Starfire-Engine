#pragma once
#include "entityComponentManager.h"
#include "components.h"
#include "GLFW/glfw3.h"
#include <unordered_map>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraManager {
public:
    CameraManager(ECS::EntityComponent& ecs, int windowSettingsEntity)
        : ecs(ecs), windowSettingsEntity(windowSettingsEntity), firstMouse(true), lastX(0.0f), lastY(0.0f), mouseEnabled(false) {}


    void setMouseCallback(GLFWwindow* window)
    {
        // Store 'this' pointer in the window user pointer
        glfwSetWindowUserPointer(window, this);

        // Set the static cursor position callback
        glfwSetCursorPosCallback(window, CameraManager::mouseCallback);

        
    }

    void addCamera(int cameraEntity, const std::string& cameraIdentifier) {
        camera_map[cameraIdentifier] = cameraEntity;
    }

    void setActiveCamera(const std::string& cameraIdentifier) {
        if (camera_map.find(cameraIdentifier) != camera_map.end()) {
            activeCamera = cameraIdentifier;
            firstMouse = true;  // Reset firstMouse to true when a new camera is activated
        }
        else {
            std::cerr << "Error: Camera identifier '" << cameraIdentifier << "' not found!" << std::endl;
        }
    }

    int getActiveCameraEntity() {
        if (activeCamera.empty()) {
            std::cerr << "Error: No active camera set!" << std::endl;
            return -1;  // Return -1 or an invalid entity ID
        }

        auto it = camera_map.find(activeCamera);
        if (it != camera_map.end()) {
            return it->second;
        }
        else {
            std::cerr << "Error: Camera component not found for the active camera!" << std::endl;
            return -1;  // Return -1 if not found
        }
    }

    std::string getActiveCameraIdentifier() const {
        return activeCamera;
    }
    

    void enableMouseInput(GLFWwindow* window, CameraManager& cameraManager) {
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
            cameraManager.mouseEnabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // Show the cursor
        }
    }

    void disableMouseInput(GLFWwindow* window, CameraManager& cameraManager) {
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
            cameraManager.mouseEnabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Hide the cursor
        }
    }

    bool isMouseEnabled() const {
        return mouseEnabled;
    }

private:
    int windowSettingsEntity;
    ECS::EntityComponent& ecs;
    std::string activeCamera;
    std::unordered_map<std::string, int> camera_map;
    bool firstMouse;
    float lastX, lastY;
    bool mouseEnabled;  // New variable to track whether the mouse is enabled

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        
        CameraManager* cameraManager = static_cast<CameraManager*>(glfwGetWindowUserPointer(window));
        if (cameraManager)
        {
            cameraManager->handleMouseMovement(window, xpos, ypos);
        }
    }

    void handleMouseMovement(GLFWwindow* window, double xpos, double ypos) {
        // Prevent camera movement if the mouse is enabled (visible and free-moving)
        if (mouseEnabled || ImGui::GetIO().WantCaptureMouse) {
            return;
        }

        if (activeCamera.empty()) {
            std::cerr << "Error: No active camera set for mouse movement!" << std::endl;
            return;
        }

        auto* windowSettings = ecs.getComponent<components::windowSettingsComponent>(windowSettingsEntity);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        int cameraEntity = getActiveCameraEntity();
        if (cameraEntity == -1) return;

        components::CameraComponent* camera = ecs.getComponent<components::CameraComponent>(cameraEntity);
        if (camera) {
            xoffset *= camera->mouseSensitivity;
            yoffset *= camera->mouseSensitivity;

            camera->yaw += xoffset;
            camera->pitch += yoffset;

            // Constrain pitch
            if (camera->pitch > 89.0f) camera->pitch = 89.0f;
            if (camera->pitch < -89.0f) camera->pitch = -89.0f;

            // Update camera front vector
            glm::vec3 front;
            front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
            front.y = sin(glm::radians(camera->pitch));
            front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
            camera->front = glm::normalize(front);
        }
        else {
            std::cerr << "Error: Camera component not found for the active camera!" << std::endl;
        }
    }
};


void updateViewMatrix(ECS::EntityComponent& ecs, CameraManager& cameraManager, glm::mat4& view) {
    int cameraEntity = cameraManager.getActiveCameraEntity();
    if (cameraEntity != -1) {
        auto* camera = ecs.getComponent<components::CameraComponent>(cameraEntity);
        if (camera) {
            view = camera->viewMatrix();  // Update the global view matrix
        }
    }
}

void moveForward(ECS::EntityComponent& ecs, CameraManager& cameraManager, int timeEntity) {
    int activeCamera = cameraManager.getActiveCameraEntity();
    auto* camera = ecs.getComponent<components::CameraComponent>(activeCamera);
    auto* timeComponent = ecs.getComponent<components::deltaTimeComponent>(timeEntity);
    if (camera) {
        camera->position += camera->movementSpeed * timeComponent->deltaTime * camera->front;
    }
}

void moveBackward(ECS::EntityComponent& ecs, CameraManager& cameraManager, int timeEntity) {
    int activeCamera = cameraManager.getActiveCameraEntity();
    auto* camera = ecs.getComponent<components::CameraComponent>(activeCamera);
    auto* timeComponent = ecs.getComponent<components::deltaTimeComponent>(timeEntity);
    if (camera) {
        camera->position -= camera->movementSpeed * timeComponent->deltaTime * camera->front;
    }
}

void moveLeft(ECS::EntityComponent& ecs, CameraManager& cameraManager, int timeEntity) {
    int activeCamera = cameraManager.getActiveCameraEntity();
    auto* camera = ecs.getComponent<components::CameraComponent>(activeCamera);
    auto* timeComponent = ecs.getComponent<components::deltaTimeComponent>(timeEntity);
    if (camera) {
        camera->position -= glm::normalize(glm::cross(camera->front, camera->up)) * camera->movementSpeed * timeComponent->deltaTime;
    }
}

void moveRight(ECS::EntityComponent& ecs, CameraManager& cameraManager, int timeEntity) {
    int activeCamera = cameraManager.getActiveCameraEntity();
    auto* camera = ecs.getComponent<components::CameraComponent>(activeCamera);
    auto* timeComponent = ecs.getComponent<components::deltaTimeComponent>(timeEntity);
    if (camera) {
        camera->position += glm::normalize(glm::cross(camera->front, camera->up)) * camera->movementSpeed * timeComponent->deltaTime;
    }
}