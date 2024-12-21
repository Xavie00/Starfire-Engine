#pragma once
#include "entityComponentManager.h"
#include "GLFW/glfw3.h"
#include "components.h"

void updateDeltaTime(ECS::EntityComponent& ecs, int timeDeltaEntity) {
    auto* timeComponent = ecs.getComponent<components::deltaTimeComponent>(timeDeltaEntity);
    if (timeComponent) {
        float currentFrameTime = glfwGetTime();
        timeComponent->deltaTime = currentFrameTime - timeComponent->lastFrameTime;
        timeComponent->lastFrameTime = currentFrameTime;
    }
}