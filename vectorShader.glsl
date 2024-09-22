#version 330 core

layout (location = 0) in vec3 aPos;  // Vertex position
layout (location = 1) in vec3 aColor; // Vertex color
layout (location = 2) in vec2 aTexCoord; // Texture coordinate

out vec3 ourColor; // Output color for fragment shader
out vec2 TexCoord; // Output texture coordinate for fragment shader

// Uniforms for transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Apply the transformation matrices
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Pass the color and texture coordinate to the fragment shader
    ourColor = aColor;
    TexCoord = aTexCoord;
}