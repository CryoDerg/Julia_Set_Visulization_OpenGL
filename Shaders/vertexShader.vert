#version 330 core
layout (location = 0) in vec2 aPos;
out vec3 position;

uniform vec2 offset;

void main() {
	gl_Position = vec4(aPos.xy, 0.0, 1.0);
	position = vec3(aPos, 0.0);
}