#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 v_TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    v_TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D ourTexture;

void main() {
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    FragColor = texture(ourTexture, v_TexCoord);
}