#version 430 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float textureId;
 
uniform vec3 cameraPos;
uniform mat4 model;

uniform bool _Batch;
uniform float _Time = 1.0;
uniform float _ZFar = 2.0;

out vec3 outColor;
out vec3 outPixelPos;
out vec3 outNormal;
out vec2 outTexCoord;
out float outTexId;

void main() {
  outTexId = textureId;
  outColor = vec3(1.0,1.0,1.0);
  outTexCoord = texCoord;

  vec4 result = vec4(vec3(position,1.0), 1.0);
  if(_Batch) {
    result.x = result.x / _ZFar;
    result.y = result.y / _ZFar;
    result.z = result.z / _ZFar;
    gl_Position = result + vec4(cameraPos, 1.0);
  } else {
    vec4 result = model * vec4(vec3(position,1.0), 1.0);
    gl_Position = result + vec4(cameraPos,1.0);
  }
}