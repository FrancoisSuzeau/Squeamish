#version 460 core

layout (location = 0) in vec3 obj_pos;
layout (location = 1) in vec2 texture_coord;

uniform mat4 model;
out vec2 TexCoords;

void main()
{
    TexCoords = texture_coord;
    gl_Position = model * vec4(obj_pos, 1.0);
    
}