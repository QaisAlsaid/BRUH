#version 330 core

in vec2 tux_coord;

uniform vec4 u_tint_color;
uniform sampler2D u_tux;

out vec4 frag_color;

void main()
{
  frag_color = texture(u_tux, tux_coord) * u_tint_color;
}
