#version 410

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 color;

layout(location=4) in float weightPrev;
layout(location=5) in float weightCur;
layout(location=6) in float weightNext;

layout(location=20) out vec2 texCoord0;
layout(location=21) out vec3 normal0;
layout(location=22) out vec3 color0;
layout(location=23) out vec4 viewSpace;

uniform mat4 projection;
struct LinksMV {
	mat4 prev;
	mat4 cur;
	mat4 next;
};

uniform LinksMV linksMV;

vec4 getWeigtenedPos(vec4 prevPos, vec4 curPos, vec4 nextPos) {
	return weightPrev * linksMV.prev * prevPos +
		weightCur * linksMV.cur * curPos + 
		weightNext * linksMV.next * nextPos;
}

void main() {
	texCoord0 = texCoord;

	vec4 normal4 = vec4(normal, 0.0);

	normal0 = (getWeigtenedPos(normal4, normal4, normal4)).xyz;
	color0 = color;

	vec4 prevPos = vec4(position.x, position.y, -position.z, 1.0);
	vec4 curPos = vec4(position, 1.0);
	vec4 nextPos = vec4(position.x, position.y, -position.z, 1.0);

	vec4 prevPos0 = vec4(0.0, 0.0, -position.z, 1.0);
	vec4 curPos0 = vec4(0.0, 0.0, position.z, 1.0);
	vec4 nextPos0 = vec4(0.0, 0.0, -position.z, 1.0);

	vec4 curPosNoWeight = linksMV.cur * curPos;
	vec4 curPos0NoWeight = linksMV.cur * curPos0;

	vec4 posWeight = getWeigtenedPos(prevPos, curPos, nextPos);
	vec4 pos0Weight = getWeigtenedPos(prevPos0, curPos0, nextPos0);
	vec4 center = linksMV.cur * ((position.z > 0) ? vec4(0,0,1,1) : vec4(0,0,-1,1));
	vec4 base = 0.8 * pos0Weight + 0.2* center;
	float radius = distance(curPosNoWeight, curPos0NoWeight);
	float length = length(posWeight - curPos0NoWeight);
	
	if(length > 0.001) {
		vec4 dir = normalize(posWeight - base);
		vec4 mvMultPos = base + radius * dir;
		viewSpace = mvMultPos;
		gl_Position = projection * mvMultPos;
	} else {
		viewSpace = posWeight;
		gl_Position = projection * posWeight;
	}
}

// void main() {
// 	texCoord0 = texCoord;

// 	vec4 normal4 = vec4(normal, 0.0);

// 	normal0 = (linksMV.cur * normal4).xyz;
// 	color0 = color;

// 	gl_Position = projection * linksMV.cur * vec4(position,1);
// }