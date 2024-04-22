#version 330 core


in vec2 v_tux_coord;
in vec4 v_color;
in float v_tux_idx;

//Todo: remove it :
uniform vec4 u_tint_color;
uniform sampler2D u_textures [8];

out vec4 frag_color;

void main()
{
  int idx = int(v_tux_idx);
  switch(idx)
  {
  case 0:
    frag_color = texture(u_textures[0], v_tux_coord) * v_color;
    break;
  case 1:
    frag_color = texture(u_textures[1], v_tux_coord) * v_color;
    break;
  case 2:
    frag_color = texture(u_textures[2], v_tux_coord) * v_color;
    break;
  case 3:
    frag_color = texture(u_textures[3], v_tux_coord) * v_color;
    break;
  case 4:
    frag_color = texture(u_textures[4], v_tux_coord) * v_color;
    break;
  case 5:
    frag_color = texture(u_textures[5], v_tux_coord) * v_color;
    break;
  case 6:
    frag_color = texture(u_textures[6], v_tux_coord) * v_color;
    break;
  case 7:
   frag_color = texture(u_textures[7], v_tux_coord) * v_color;
    break;
  }
}
