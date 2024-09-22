#pragma once

#include <unordered_map>
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>


class ShaderManager {
public:
    // Load and store a shader program
    void loadShader(const std::string& name, const char* vertexPath, const char* fragmentPath) {
        unsigned int shaderID = loadShaderFromFile(vertexPath, fragmentPath);
        shaders[name] = shaderID;
    }

    // Get a shader program by name
    unsigned int getShader(const std::string& name) const {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            return it->second;
        }
        std::cerr << "Shader not found: " << name << std::endl;
        return 0; // Or throw an exception if preferred
    }

private:
    std::unordered_map<std::string, unsigned int> shaders;

    // Helper function to compile and link shaders
    unsigned int loadShaderFromFile(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;

        // Read shader code from files
        try {
            std::ifstream vShaderFile(vertexPath);
            std::ifstream fShaderFile(fragmentPath);

            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            return 0;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int success;
        char infoLog[1024];

        // Compile vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Compile fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Link shaders into a program
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertex);
        glAttachShader(shaderProgram, fragment);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Clean up shaders as they're linked into the program
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return shaderProgram;
    }
};
