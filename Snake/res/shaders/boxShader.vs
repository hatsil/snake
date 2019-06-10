#version 410

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 color;

layout(location=20) out vec2 texCoord0;
layout(location=21) out vec3 normal0;
layout(location=22) out vec3 color0;
layout(location=23) out vec4 viewSpace;

uniform mat4 projection;
uniform mat4 MV;

void main() {
	viewSpace = MV * vec4(position, 1.0);
	texCoord0 = texCoord;
	normal0 = (MV * vec4(normal, 0.0)).xyz;
	color0 = color;
	gl_Position = projection * MV * vec4(position, 1.0);
}