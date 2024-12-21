#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "entityComponentManager.h"
#include "components.h"




class WindowManager {
public:

    bool createWindow(ECS::EntityComponent ecs, int entity) {
        // Initialize GLFW

        auto windowSettings = ecs.getComponent<components::windowSettingsComponent>(entity);

        if (!glfwInit()) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // Set GLFW options for OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a GLFW window
        GLFWwindow* window = glfwCreateWindow(windowSettings->windowWidth, windowSettings->windowHeight, windowSettings->windowTitle, NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);

        // Initialize GLAD to load OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        // Store the window and its settings
        window_map[windowSettings->windowIdentifier] = window;

        // ImGui context creation
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Set GLFW backend for ImGui
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Make sure ImGui handles keyboard inputs
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

       

        return true;
    }

    // Get GLFWwindow* by windowIdentifier (std::string)
    GLFWwindow* getWindowByIdentifier(const std::string& identifier) const {
        auto it = window_map.find(identifier);
        if (it != window_map.end()) {
            return it->second;
        }
        else {
            std::cout << "Window identifier not found" << std::endl;
            return nullptr;
        }
    }

    ~WindowManager() {
        // Cleanup ImGui context
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Terminate GLFW
        glfwTerminate();
    }

private:
    std::unordered_map<std::string, GLFWwindow*> window_map;                // Map from window identifier to GLFWwindow*
};