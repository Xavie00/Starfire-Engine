#pragma once
#include "cachedECS.h"
#include "components.h"
#include "GLFW/glfw3.h"
#include "window.h"

// Global view and projection matrices
glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


void updateCameraMatrices(ECS::EntityComponent& ecs) {
    for (int entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
        if (ecs.hasComponents<components::CameraComponent>(entity)) {
            auto* cameraComponent = ecs.getComponent<components::CameraComponent>(entity);

            // Update global view and projection matrices
            view = cameraComponent->viewMatrix();
            projection = cameraComponent->projectionMatrix();

            // Update aspect ratio if needed (assuming you update it on window resize)
            cameraComponent->aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        }
    }
}

void processInput(GLFWwindow* window, components::CameraComponent& camera, float deltaTime) {
    float cameraSpeed = camera.movementSpeed * deltaTime;

    // Only process camera movement if the mouse and camera are enabled

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;


}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    components::CameraComponent* camera = reinterpret_cast<components::CameraComponent*>(glfwGetWindowUserPointer(window));
    if (camera) {
        xoffset *= camera->mouseSensitivity;
        yoffset *= camera->mouseSensitivity;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        front.y = sin(glm::radians(camera->pitch));
        front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        camera->front = glm::normalize(front);

    }
}

void translateCameraW() {

}