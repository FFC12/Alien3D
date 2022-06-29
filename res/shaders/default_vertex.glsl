#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 normal;
layout (location = 2) in vec2 texCoord;

{{ common.glsl.h }}

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 outColor;
out vec2 outNormal;
out vec2 outTexCoord;

void main() {
    outNormal = normal;
    outTexCoord = texCoord;
    gl_Position = proj * view * model * vec4(position, 1.0);
                                            outColor = vec3(1.0, 1.0, 1.0);
}