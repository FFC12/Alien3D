#version 330 core
out vec4 FragColor;

in vec3 outColor;
in vec2 outNormal;
in vec2 outTexCoord;

uniform sampler2D _Diffuse;
uniform sampler2D _NormalMap;
uniform sampler2D _HeightMap;
uniform sampler2D _SpecularMap;

void main() {
    FragColor = texture(_Diffuse, outTexCoord) * vec4(outColor, 1.0);
}