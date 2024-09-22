#version 330 core

in vec3 ourColor; // Input color from vertex shader
in vec2 TexCoord; // Input texture coordinate from vertex shader

out vec4 FragColor; // Output color of the fragment

// Texture sampler
uniform sampler2D texture1;

void main() {
    // Sample the texture at the given coordinate
    vec4 texColor = texture(texture1, TexCoord);
    
    // Combine texture color with the vertex color
    FragColor = texColor * vec4(ourColor, 1.0);
}
//add texColor * vec4(ourColor, 1.0); for colors!