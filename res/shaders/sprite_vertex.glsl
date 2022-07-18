#version 430 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform vec3 cameraPos;
uniform mat4 model;

uniform bool _Batch;
uniform float _Time = 1.0;
uniform float _ZFar = 5.0;
uniform float _Width = 1920;
uniform float _Height = 1024;

out vec3 outColor;
out vec3 outPixelPos;
out vec3 outNormal;
out vec2 outTexCoord;
out float outHeight;
out float outWidth;

void main() {
  outColor = vec3(1.0,1.0,1.0);
  outTexCoord = texCoord;
  outHeight = _Height;
  outWidth = _Width;

  vec4 result = model * vec4(vec3(position,1.0), 1.0);

  //if(_Height < _Width) {
  //  result.x = result.x * (_Height / _Width);
  //} else {
  //  result.y = result.y * (_Width / _Height);
  //}

  vec4 p = result + vec4(cameraPos,1.0);
  gl_Position = p;
}