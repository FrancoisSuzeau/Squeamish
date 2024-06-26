#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;
uniform bool horizontal;

uniform float weight[5] = float[] (0.2270270270, 0.1945945945, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
    vec2 tex_offset = 1.0f / textureSize(texture0, 0);
    vec3 result = texture(texture0, TexCoords).rgb * weight[0];

    if(horizontal)
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(texture0, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(texture0, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; i++)
        {
            result += texture(texture0, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(texture0, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    FragColor = vec4(result, 1.0);
}