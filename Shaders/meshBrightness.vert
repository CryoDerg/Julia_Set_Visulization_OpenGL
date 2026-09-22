#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in float brightness;
out vec4 color;

void main() {
	gl_Position = vec4(position.xyz, 1.0);
	color = vec4(brightness, 0.0, 0.5, 1.0);
}