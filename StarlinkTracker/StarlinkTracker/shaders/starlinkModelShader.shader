#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_TexCoord;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    v_TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

out vec4 FragColor;

in vec2 v_TexCoord;

void main() {
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    FragColor = texture(texture_diffuse0, v_TexCoord);
}