#version 400 core
in vec3 position;
out vec4 gl_FragColor;

uniform vec2 z;
uniform int steps = 10;
uniform float zoom = 0.1;
uniform vec2 offset = vec2(-2, 20);
uniform float time;


vec2 complexPow(vec2 base, float ex) {
	float r = length(base);
	if (r == 0.0) return vec2(0.0, 0.0);
	float theta = atan(base.y, base.x);

	float rp = pow(r, ex);
	float angle = ex * theta;

	return vec2(rp * cos(angle), rp * sin(angle));
}

float recur(int steps, vec2 Z, vec2 C) {
	vec2 next = Z;
	//return length(compExp(Z, 2) + C);
	for (int i = steps; i > 0; i--) {
		if (length(next) > 20) {
			return i / float(steps);
		}
		// y component is imaginary
		next = complexPow(next, 2) + C;
	}
	return 0.0;
}

void main() {
	vec2 juliaC = 0.1*pow(time,0.5) * complexPow(vec2(0.0, 1.0), time) - vec2(0.125,0);
	vec2 mandleC = vec2(offset + (position.xy / zoom));
	if (length(juliaC - mandleC) < 0.03) {
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else {
		double divRate = recur(steps, vec2(0,0), mandleC);
		if (divRate > 0.8) {
			discard;
		}

		gl_FragColor = vec4(0.8 - divRate, 0.8 - divRate,  1.2 - divRate, 1.0);
		//gl_FragColor = vec4(offset.x + (position.x / zoom), offset.y + (position.y / zoom),  1.2 - divRate, 1.0);
	}
}

