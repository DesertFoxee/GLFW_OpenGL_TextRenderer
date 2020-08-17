#version 430

layout(location =0 ) in vec3 position;

uniform mat4 vmMatrix;
uniform mat4 projMatrix;

out vec4 frag_color_in;

void main(void){ 
	gl_Position = projMatrix* vmMatrix * vec4(position , 1.0); 

	frag_color_in = vec4(position,1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}
