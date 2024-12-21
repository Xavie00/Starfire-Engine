#pragma once
#include <unordered_map>
#include <string>
#include "entityComponentManager.h"
#include "eventManager.h"
#include "modelManager.h"
#include "textureManager.h"
#include "shaderManager.h"
#include "windowManager.h"
#include "cameraManager.h"

class GameObjectManager {
public:
    
    // Create a new game object with a unique name
    void createGameObject(const std::string& name) {
        if (game_objects.find(name) != game_objects.end()) {
            std::cout << "GameObject with name \"" << name << "\" already exists!\n";
            return;
        }

        // Create a new entity using ECS and store its ID
        int entityID = ecs.createEntity();
        game_objects[name] = entityID;

        std::cout << "Created GameObject: " << name << " with ID: " << entityID << "\n";
    }

    // Retrieve the ID of a game object by name
    int getGameObjectID(const std::string& name) const {
        auto it = game_objects.find(name);
        if (it != game_objects.end()) {
            return it->second;
        }
        std::cerr << "Error: GameObject with name \"" << name << "\" not found.\n";
        return -1;
    }
    ECS::EntityComponent ecs;  // The ECS system for entity management
 

private:
    std::unordered_map<std::string, int> game_objects;  // Map game object names to entity IDs
    

    
};
