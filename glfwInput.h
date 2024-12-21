#pragma once
#include <GLFW/glfw3.h>
#include "components.h"
#include "entityComponentManager.h"
#include "eventManager.h"


bool key_pressed(GLFWwindow* window, int key) {
    ImGuiIO& io = ImGui::GetIO();

    // Forward key event to ImGui
    ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_PRESS, 0);

    // Only check for key press if ImGui is not capturing keyboard input
    if (!io.WantCaptureKeyboard && glfwGetKey(window, key) == GLFW_PRESS) {
        return true;
    }
    return false;
}

bool key_released(GLFWwindow* window, int key) {
    ImGuiIO& io = ImGui::GetIO();

    // Forward key event to ImGui
    ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_RELEASE, 0);

    // Only check for key release if ImGui is not capturing keyboard input
    if (!io.WantCaptureKeyboard && glfwGetKey(window, key) == GLFW_RELEASE) {
        return true;
    }
    return false;
}
