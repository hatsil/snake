#version 410

layout(location=20) in vec2 texCoord0;
layout(location=21) in vec3 normal0;
layout(location=22) in vec3 color0;
layout(location=23) in vec4 viewSpace;

uniform vec3 camPos;
uniform sampler2D sampler;

const vec3 fogColor = vec3(0.0, 0.5, 1);
const float FogDensity = 0.015;
const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor = vec3(0.2, 0.2, 0.2);
const float ambientFactor = 0.3;
const vec3 lightDirection = vec3(1,1,1);

void main()
{
    //gl_FragColor = texture2D(sampler, texCoord0);

    // Blinn Phong
    vec3 L = normalize(lightDirection);
    vec3 diffuse = DiffuseLight * max(dot(-L, normal0), 0);
    //vec4 hitPosition = (Normal * vec4(viewSpace.xyz, 1.0));
    vec3 v = normalize(camPos - viewSpace.xyz);
    vec3 r = reflect(L, normalize(normal0));
    float specular = max(dot(v,r), 0);
    vec3 lightIntensity = diffuse + pow(specular, 10) * RimColor;

    // Texture
    vec3 texture = texture2D(sampler, texCoord0).rgb;
    vec3 lightColor = lightIntensity + texture;

    // Fog Distance
    float dist = length(viewSpace); // Range based
    float fogFactor = 1.0 / exp((dist * FogDensity) * (dist * FogDensity));
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 foggedColor = mix(fogColor, lightColor, fogFactor);
    gl_FragColor = clamp(vec4(foggedColor, 1.0), 0.0, 1.0);
}
