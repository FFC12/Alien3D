#version 430 core
out vec4 FragColor;

in vec3 outColor;
in vec3 outPixelPos;
in vec3 outNormal;
in vec2 outTexCoord;
in float outTexId;

{{ common.glsl.h }}

uniform bool _HasLight;

uniform sampler2D _Sprites[32];
uniform bool _Animation = false;
uniform bool _Flip = false;
uniform vec2 _FrameI;
uniform float _FrameSizeX = 32;
uniform float _FrameSizeY = 32;
uniform float _Width = 1920;
uniform float _Height = 1024;

void main() {
  vec2 uv = outTexCoord;

  if(_Animation) {
    float Wn = _FrameSizeX / _Width * _FrameI.x;
    if(Wn > 1.0) Wn = 1.0;

    float Hn = _FrameSizeY / _Height * _FrameI.y;
    if(Hn > 1.0) Hn = 1.0;

    uv.x = uv.x / (_Width / _FrameSizeX);
    uv.y = uv.y / (_Height / _FrameSizeY);
    uv.x += Wn;
    uv.y += Hn + 0.01;

    if(_Flip)
      uv.x = 1.0 - uv.x;

    vec4 p = texture(_Sprites[int(outTexId)], uv);
    if (p.a < 0.01) {
      discard;
    }

    FragColor = p;
  }

  vec4 p = texture(_Sprites[int(outTexId)], uv);
//   vec4 p = vec4(outTexId / 1.0, outTexId,outTexId,1.0);

  if (p.a < 0.1) {
    discard;
  }

  FragColor = p;
}