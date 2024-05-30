#version 450 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec4 background_color;
uniform bool there_is_light;
uniform float ambiant_strength;
uniform int specular_shininess;
uniform float specular_strength;

struct Light 
{
    vec3 position;
    vec4 inner_color;
    bool is_textured;
    bool mixe_texture_color;
    sampler2D texture;
};

uniform Light src_light;

uniform bool is_light_source;
uniform vec3 camera_pos;

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
} fs_in;

vec4 GetMixedColor(vec4 object_texture, vec4 color, bool mixe)
{
    if(mixe)
    {
        return vec4(mix(object_texture.rgb, color.rgb, color.a) * object_texture.a, 1.0);
    }
    else
    {
        return object_texture;
    }
}

void main()
{
    
    //Case if the fragment is part of a light source object
    if(is_light_source)
    {
        FragColor = background_color;
        float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
        if(brightness > 0.f)
        {
            BrightColor = vec4(FragColor.rgb, 1.0);
        }
        else
        {
            BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
    //Case if the fragment is not part of a light source object -> calculate complex object light effect
    else
    {
        if(there_is_light)
        {
            vec3 light_color = src_light.inner_color.rgb;
            if(src_light.is_textured)
            {
                vec4 light_fragment = texture(src_light.texture, vec2(1.f));
                light_color = GetMixedColor(light_fragment, src_light.inner_color, src_light.mixe_texture_color).rgb;
            }

            //Ambiant
            vec3 ambiant = ambiant_strength * light_color;
            
            //Diffuse
            vec3 norm = normalize(fs_in.Normal);
            vec3 light_dir = normalize(src_light.position - fs_in.FragPos);
            float diff = max(dot(light_dir, norm), 0.f);
            vec3 diffuse = diff * light_color;

            //Specular
            vec3 view_dir = normalize(camera_pos - fs_in.FragPos);
            vec3 reflect_dir = reflect(-light_dir, norm);  
            float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_shininess);
            vec3 specular = specular_strength * spec * light_color;  

            //Pass to framebuffer 0 (normal output)
            vec3 result = (ambiant + diffuse + specular) * background_color.rgb;
            FragColor = vec4(result, background_color.a);
            float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

            //Pass to framebuffer 1 (bright output)
            if(brightness > 1.f)
            {
                BrightColor = vec4(FragColor.rgb, 1.0);
            }
            else
            {
                BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
            }
            
        }
        else
        {
            FragColor = background_color;
        }
        
    }
    
}
