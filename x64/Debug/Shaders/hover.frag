#version 460 core

out vec4 FragColor;
uniform bool render_line;

void main()
{
    FragColor = vec4(1.f, 1.f, 1.f, render_line);
}