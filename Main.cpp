#include "initializations.h"
//hello
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void close_window(GLFWwindow* window);

int main() {


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Me After My Code Works", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Initialize ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glEnable(GL_DEPTH_TEST);


    // Initialize ShaderManager
    ShaderManager shaderManager;
    shaderManager.loadShader("default", "vectorShader.glsl", "fragmentShader.glsl");


    TextureManager textureManager;
    textureManager.loadTexture("joke", "Creeper.png");

    ECS::EntityComponent ecs;

    // Create an entity and add components
    auto entity1 = ecs.createEntity();

    // Add position, color, texture coordinate, and texture components to the entity
    ecs.addComponent(entity1, components::VertexComponent{
        {
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
        }
        });
    ecs.addComponent(entity1, components::IndexComponent{
        { // Front face
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
    22, 23, 20}
        });

    ecs.addComponent(entity1, components::TextureComponent{ textureManager.getTexture("joke"), "joke" });
    ecs.addComponent(entity1, components::ShaderComponent{ "default" });
    ecs.addComponent(entity1, components::ModelMatrixComponent{});

    

    auto* modelMatrix = ecs.getComponent<components::ModelMatrixComponent>(entity1);

    EventBus eventBus;

    eventBus.listenForEvent("close_window", std::bind(close_window, window));

    TriggerManager triggerManager;

    triggerManager.addTrigger("key_escape_pressed", std::bind(key_pressed, window, GLFW_KEY_ESCAPE));
    triggerManager.addEventToTrigger("key_escape_pressed", "close_window");

    while (!glfwWindowShouldClose(window)) {
        triggerManager.checkAndTriggerEvents(eventBus);

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ImGui interface
        ImGui::Begin("Create a Cube");

        const char* items[] = { "Item 1", "Item 2", "Item 3" };
        static int item_current = 0;
        if (ImGui::BeginCombo("Dropdown", items[item_current])) {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                const bool is_selected = (item_current == n);
                if (ImGui::Selectable(items[n], is_selected)) {
                    item_current = n;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::End();
        ImGui::Render();

        // Clear buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update camera


        // Render entities
        renderEntities(ecs, shaderManager, textureManager);

        // Render ImGui data
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
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

