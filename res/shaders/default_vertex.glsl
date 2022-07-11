#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform bool _IsSprite;
uniform vec3 cameraPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


uniform float _Time = 1.0;

out vec3 outColor;
out vec3 outPixelPos;
out vec3 outNormal;
out vec2 outTexCoord;

void main() {
    outTexCoord = texCoord;

    if(!_IsSprite) {
        outPixelPos = vec3(model * vec4(position, 1.0));

        // this costs a lot, you may consider to done that formula on CPU-side
        outNormal = mat3(transpose(inverse(model))) * normal;
    }
    outColor = vec3(1.0, 1.0, 1.0);

    if(_IsSprite) {
        vec4 result = model * vec4(position, 1.0);
        gl_Position = result + vec4(cameraPos,1.0);
    } else {
        gl_Position = proj * view * model * vec4(position, 1.0);
    }
}