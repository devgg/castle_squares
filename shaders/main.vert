#version 420 core

uniform mat4 view;
uniform mat4 proj;

layout(location = 0) in vec3 position;

out vec3 vert_color;

void main() {
   vert_color = vec3(1.0, 1.0, 1.0);
   gl_Position = proj * view * vec4(position, 1.0);
}