#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <iostream>

#include "stb_image.h"

class TextureManager {
public:
    // Load and store a texture
    void loadTexture(const std::string& name, const std::string& filepath) {
        stbi_set_flip_vertically_on_load(true);
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set the texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the image using stb_image
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Failed to load texture: " << filepath << std::endl;
        }
        stbi_image_free(data);

        // Store the texture ID
        textures[name] = textureID;
    }

    // Get a texture ID by name
    unsigned int getTexture(const std::string& name) const {
        auto it = textures.find(name);
        if (it != textures.end()) {
            return it->second;
        }
        std::cerr << "Texture not found: " << name << std::endl;
        return 0; // Return a default or error texture ID if preferred
    }

private:
    std::unordered_map<std::string, unsigned int> textures;
};
