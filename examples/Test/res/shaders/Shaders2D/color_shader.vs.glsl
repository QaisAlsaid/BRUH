#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 nothing;


uniform mat4 u_proj_view;
uniform mat4 u_trans;
uniform vec4 u_color;

out vec4 color_in;

void main()
{
  color_in = u_color;
  gl_Position = u_proj_view  * u_trans * vec4(vert_pos, 1.0);
}
