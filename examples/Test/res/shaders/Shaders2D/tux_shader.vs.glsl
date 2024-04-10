#version 330 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_tux_coord;

uniform mat4 u_proj_view;
uniform mat4 u_trans;

out vec2 tux_coord;

void main()
{
  gl_Position = u_proj_view * u_trans * vec4(i_pos, 1.0);
  tux_coord = i_tux_coord;
}
