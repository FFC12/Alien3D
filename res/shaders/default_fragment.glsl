#version 430 core
out vec4 FragColor;

in vec3 outColor;
in vec3 outPixelPos;
in vec3 outNormal;
in vec2 outTexCoord;

{{ common.glsl.h }}

uniform bool _HasLight;

uniform sampler2D _Diffuse;
uniform sampler2D _NormalMap;
uniform sampler2D _HeightMap;
uniform sampler2D _SpecularMap;

void main() {
  if (_HasLight) {
    vec3 lightColor = AlienBasicLighting(outNormal, outPixelPos, true);
    FragColor = texture(_Diffuse, outTexCoord) * vec4(outColor, 1.0) *
                vec4(lightColor, 1.0);
//             FragColor = vec4(outColor, 1.0) * vec4(lightColor,1.0);
  } else {
    vec2 uv = outTexCoord;
    vec2 distort = DistortedUV();
    vec4 p = texture(_Diffuse, distort);

    // discrad alpha
    if (p.a < 0.1) {
      discard;
    }

    FragColor = p; //mix(p, vec4(outColor, 1.0) * SimpleWaterWave(), 0.6);
  }
}