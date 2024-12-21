#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "cameraManager.h"
#include "entityComponentManager.h"
#include "shaderManager.h"
#include "textureManager.h"
#include "eventManager.h"
#include "windowManager.h"

#include "renderingSystem.h"
#include "timeSystem.h"

#include "glfwInput.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void close_window(GLFWwindow* window);

glm::mat4 view;
glm::mat4 projection;

int main() {
    ECS::EntityComponent ecs;

    WindowManager windowManager;
    int windowEntity = ecs.createEntity();
    ecs.addComponent(windowEntity, components::windowSettingsComponent{});

    windowManager.createWindow(ecs, windowEntity);
    auto* windowData = ecs.getComponent<components::windowSettingsComponent>(windowEntity);
    auto window = windowManager.getWindowByIdentifier(windowData->windowIdentifier);

    CameraManager cameraManager(ecs, windowEntity);
    cameraManager.setMouseCallback(window);

    int cameraEntity = ecs.createEntity();
    ecs.addComponent(cameraEntity, components::CameraComponent{});

    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
    auto projection = glm::perspective(glm::radians(45.0f), (float)windowData->windowWidth / (float)windowData->windowHeight, 0.1f, 100.0f);

    // Register the camera with the manager
    cameraManager.addCamera(cameraEntity, "mainCamera");
    cameraManager.setActiveCamera("mainCamera");

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    glEnable(GL_DEPTH_TEST);


    // Initialize ShaderManager
    ShaderManager shaderManager;
    shaderManager.loadShader("default", "vectorShader.glsl", "fragmentShader.glsl");


    TextureManager textureManager;
    textureManager.loadTexture("joke", "Creeper.png");

    Cube cube;

    // Create an entity and add components
    auto entity1 = ecs.createEntity();
    
    assignShapeToEntity(entity1, ecs, cube.vertices, cube.indicies);

    int timeEntity = ecs.createEntity();
    ecs.addComponent(timeEntity, components::deltaTimeComponent{});

    auto* modelMatrix = ecs.getComponent<components::ModelMatrixComponent>(entity1);

    EventBus eventBus;

    eventBus.listenForEvent("close_window", std::bind(close_window, window));
    eventBus.listenForEvent("move_forward", std::bind(moveForward, ecs, cameraManager, timeEntity));
    eventBus.listenForEvent("move_left", std::bind(moveLeft, ecs, cameraManager, timeEntity));
    eventBus.listenForEvent("move_backward", std::bind(moveBackward, ecs, cameraManager, timeEntity));
    eventBus.listenForEvent("move_right", std::bind(moveRight, ecs, cameraManager, timeEntity));
    eventBus.listenForEvent("enable_mouse", [window, &cameraManager]() {
        cameraManager.enableMouseInput(window, cameraManager);
        });

    eventBus.listenForEvent("disable_mouse", [window, &cameraManager]() {
        cameraManager.disableMouseInput(window, cameraManager);
        });



    TriggerManager triggerManager;

    triggerManager.addTrigger("key_escape_pressed", std::bind(key_pressed, window, GLFW_KEY_ESCAPE));
    triggerManager.addEventToTrigger("key_escape_pressed", "close_window");

    triggerManager.addTrigger("key_w_pressed", std::bind(key_pressed, window, GLFW_KEY_W));
    triggerManager.addTrigger("key_a_pressed", std::bind(key_pressed, window, GLFW_KEY_A));
    triggerManager.addTrigger("key_s_pressed", std::bind(key_pressed, window, GLFW_KEY_S));
    triggerManager.addTrigger("key_d_pressed", std::bind(key_pressed, window, GLFW_KEY_D));
    triggerManager.addTrigger("key_1_pressed", std::bind(key_pressed, window, GLFW_KEY_1));
    triggerManager.addTrigger("key_2_pressed", std::bind(key_pressed, window, GLFW_KEY_2));

    triggerManager.addEventToTrigger("key_w_pressed", "move_forward");
    triggerManager.addEventToTrigger("key_a_pressed", "move_left");
    triggerManager.addEventToTrigger("key_s_pressed", "move_backward");
    triggerManager.addEventToTrigger("key_d_pressed", "move_right");
    triggerManager.addEventToTrigger("key_1_pressed", "enable_mouse");
    triggerManager.addEventToTrigger("key_2_pressed", "disable_mouse");

    auto* deltaTimeComponent = ecs.getComponent<components::deltaTimeComponent>(timeEntity);

    triggerManager.checkAndTriggerEvents(eventBus);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cameraManager.enableMouseInput(window, cameraManager);

    while (!glfwWindowShouldClose(window))
    {
        // Update the delta time for smooth frame-independent movement
        updateDeltaTime(ecs, timeEntity);

        // Retrieve the updated delta time from the component
        float deltaTime = deltaTimeComponent->deltaTime;

        // Check and trigger any queued events
        triggerManager.checkAndTriggerEvents(eventBus);

        // Update the view matrix based on the currently active camera
        updateViewMatrix(ecs, cameraManager, view);

        // Clear the screen buffers: set background color and clear color and depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create an ImGui window with a button
        ImGui::Begin("Button Window");

        // Button that adds a new cube when clicked
        if (ImGui::Button("Add New Cube"))
        {
            std::cout << "Button was clicked!" << std::endl;
        }

        // End ImGui window
        ImGui::End();

        // Render the ImGui frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Render ECS entities using the shader and texture managers
        renderEntities(ecs, shaderManager, textureManager, view, projection);

        // Swap front and back buffers and poll for input events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void close_window(GLFWwindow* window) 
{
    glfwSetWindowShouldClose(window, true);
}

