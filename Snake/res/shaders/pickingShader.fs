#version 410

uniform vec3 pickingColor;

void main() {
    gl_FragColor = vec4(pickingColor, 1);
}
