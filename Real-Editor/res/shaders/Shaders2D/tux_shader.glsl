#REAL_SHADER_TYPE REAL_VERTEX
#version 330 core

layout (location = 0) in vec4 i_color;
layout (location = 1) in vec3 i_pos;
layout (location = 2) in vec2 i_tux_coord;
layout (location = 3) in float i_tux_idx;
//layout (location = 4) in int i_id;

uniform mat4 u_proj_view;
uniform mat4 u_trans;

out vec4 v_color;
out vec2 v_tux_coord;
flat out int v_tux_idx;
//out int v_id;

void main()
{
  gl_Position = u_proj_view * vec4(i_pos, 1.0);
  v_color = i_color;
  v_tux_coord = i_tux_coord;
  v_tux_idx = int(i_tux_idx);
  //v_id = i_id;
}

#REAL_SHADER_TYPE REAL_FRAGMENT
#REAL_SAMPLE_TEMPLATE frag_color = texture(u_textures[${REAL_CURRENT_SWITCH_CASE}], v_tux_coord) * v_color;

#version 330 core

layout (location = 0) out vec4 frag_color;
layout (location = 1) out int e_id;

in vec2 v_tux_coord;
in vec4 v_color;
flat in int v_tux_idx;
//in int v_id;

uniform sampler2D u_textures[${REAL_MAX_TEXTUERS}];


void main()
{
  e_id = 1;
  int idx = v_tux_idx;
  #REAL_MAX_TEXTUER_SWITCH(idx)
 if(frag_color.a < 0.001)
    discard;
}
