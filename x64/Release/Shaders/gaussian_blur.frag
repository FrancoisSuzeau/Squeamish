#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

struct Component
{
    sampler2D texture0;
    bool horizontal;
};

uniform Component component;

uniform float weight[5] = float[] (0.2270270270, 0.1945945945, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
    vec2 tex_offset = 1.0f / textureSize(component.texture0, 0);
    vec4 result = texture(component.texture0, TexCoords) * weight[0];

    if(component.horizontal)
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(component.texture0, TexCoords + vec2(tex_offset.x * i, 0.0)) * weight[i];
            result += texture(component.texture0, TexCoords - vec2(tex_offset.x * i, 0.0)) * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(component.texture0, TexCoords + vec2(0.0, tex_offset.y * i)) * weight[i];
            result += texture(component.texture0, TexCoords - vec2(0.0, tex_offset.y * i)) * weight[i];
        }
    }

    FragColor = result;
}