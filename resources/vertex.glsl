#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTex;

out vec4 color;
out vec2 TexCoord;

uniform mat4 projection;

void main() {
    color = vec4(aTex.xyx, 1.0f); 
    TexCoord = aTex; 
    gl_Position = projection * vec4(aPos.xyz, 1.0f); 
}