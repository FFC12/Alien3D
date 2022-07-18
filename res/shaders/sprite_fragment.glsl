#version 430 core
out vec4 FragColor;

in float outHeight;
in float outWidth;
in vec3 outColor;
in vec3 outPixelPos;
in vec3 outNormal;
in vec2 outTexCoord;

{{ common.glsl.h }}

uniform bool _HasLight;

uniform sampler2D _Diffuse;
uniform float _AnimWidth;
uniform float _AnimHeight;
uniform bool _Animation = false;
uniform bool _Flip = false;
uniform vec2 _FrameI;
uniform float _FrameSizeX = 32;
uniform float _FrameSizeY = 32;

void main() {
  vec2 uv = outTexCoord;

  if(_Animation) {
    float Wn = _FrameSizeX / _AnimWidth * _FrameI.x;
    if(Wn > 1.0) Wn = 1.0;

    float Hn = _FrameSizeY / _AnimHeight * _FrameI.y;
    if(Hn > 1.0) Hn = 1.0;

    uv.x = uv.x / (_AnimWidth / _FrameSizeX);
    uv.y = uv.y / (_AnimHeight / _FrameSizeY);
    uv.x += Wn;
    uv.y += Hn + 0.01;

    if(_Flip)
      uv.x = 1.0 - uv.x;

    vec4 p = texture(_Diffuse, uv);
    if (p.a < 0.01) {
      discard;
    }

    FragColor = p;
  }

  vec4 p = texture(_Diffuse, uv);

  if (p.a < 0.1) {
    discard;
  }

  FragColor = p;
}