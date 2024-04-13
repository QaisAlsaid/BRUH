#version 330 core

layout (location = 0) in vec3 i_pos;
layout (location = 3) in vec2 i_normal;

uniform mat4 u_proj_view;
uniform mat4 u_trans;

out vec2 o_normal;
out vec3 o_frag_pos;

void main()
{
  gl_Position = u_proj_view * u_trans * vec4(i_pos, 1.0);
  o_frag_pos = vec3(u_trans * vec4(i_pos, 1.0));
  o_normal = i_normal;
}
