#version 420 core

in vec2 vert_texcoord;
out vec4 frag_color;

uniform sampler2D texFramebuffer;

void main() {
	int kernel_size = 30;

	for (float i = 0; i < kernel_size; i++) {
		frag_color += texture(texFramebuffer, vec2(vert_texcoord.x + i / 2000.0, vert_texcoord.y));
		frag_color += texture(texFramebuffer, vec2(vert_texcoord.x - i / 2000.0, vert_texcoord.y));
	}
    frag_color /= 2 * kernel_size + 1;
}


