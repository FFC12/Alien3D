struct PointLight {
    vec3 lightPos;
    vec3 color;
    vec3 cameraPos;
};

struct Material {
    vec3 ambient;
    vec3 strength;
};

uniform PointLight _PointLight;
uniform int _LightType;
uniform float _Time = 1.0;

vec2 DistortedUV() {
   vec2 uv = outTexCoord;
   uv.x += cos(uv.y*5.2+_Time*1.4)/100.0;
   uv.y += sin(uv.x*5.1+_Time*1.4)/100.0;
   uv.x -= cos(uv.y*5.2+_Time*1.4)/100.0;
   uv.x -= cos(uv.x*5.2+_Time*1.4)/100.0;

   return uv;
}

/* Call distorted UV before this
 to distort texture (if you wanna blend and simple create reflections)*/
vec4 SimpleWaterWave(){
   vec2 uv = outTexCoord;

   vec4 texture_color = vec4(0.192156862745098, 0.6627450980392157, 0.9333333333333333, 1.0);
   vec4 k = vec4(_Time)*0.8;

   k.xy = uv * 7.0;
   float val1 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
   float val2 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.2));
   float val3 = length(0.5-fract(k.xyw*=mat3(vec3(-2.0,-1.0,0.0), vec3(3.0,-1.0,1.0), vec3(1.0,-1.0,-1.0))*0.5));
   vec4 color = vec4 ( pow(min(min(val1,val2),val3), 7.0) * 3.0)+texture_color;
   return color;
}

vec3 AlienBasicLighting(vec3 normal, vec3 pixelPos, bool blinn){
    float ambientVal = 0.1;
    vec3 ambient = ambientVal * _PointLight.color;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(_PointLight.lightPos - pixelPos);
    float surface = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = surface * _PointLight.color;

    float specVal = 0.5;
    vec3 viewDir = normalize(_PointLight.cameraPos - pixelPos);
    float spec = 0.0;
    if(blinn) {
        vec3 reflectDir = normalize(lightDir + _PointLight.cameraPos);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    } else {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    }
    vec3 specular = specVal * spec * _PointLight.color;

    return (ambient + diffuse + specular);
}