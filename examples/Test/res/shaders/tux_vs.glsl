#version 330 core

layout (location = 0) in vec3 posin;
layout (location = 1) in vec2 tux_coord;

uniform mat4 u_pv;
uniform mat4 u_trans;

out vec2 tux_coord_out;

void main()
{
  gl_Position = u_pv * u_trans * vec4(posin, 1.0);
  tux_coord_out = tux_coord;
}
