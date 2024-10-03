#REAL_SHADER_TYPE REAL_VERTEX
#version 330 core

layout (location = 0) in vec4 a_color;
layout (location = 1) in vec3 a_pos;
layout (location = 2) in vec2 a_tux_coord;
layout (location = 3) in float a_tux_idx;
layout (location = 4) in uint a_id;

uniform mat4 u_proj_view;
uniform mat4 u_trans;

out vec4 v_color;
out vec2 v_tux_coord;
flat out int v_tux_idx;
flat out uint v_id;

void main()
{
  gl_Position = u_proj_view * vec4(a_pos, 1.0);
  v_color = a_color;
  v_tux_coord = a_tux_coord;
  v_tux_idx = int(a_tux_idx);
  v_id = a_id;
}

#REAL_SHADER_TYPE REAL_FRAGMENT
#REAL_SAMPLE_TEMPLATE frag_color = texture(u_textures[${REAL_CURRENT_SWITCH_CASE}], v_tux_coord) * v_color;

#version 330 core

layout (location = 0) out vec4 frag_color;
layout (location = 1) out uint e_id;

in vec2 v_tux_coord;
in vec4 v_color;
flat in int v_tux_idx;
flat in uint v_id;

uniform sampler2D u_textures[${REAL_MAX_TEXTUERS}];


void main()
{
  e_id = v_id;
  int idx = v_tux_idx;
  #REAL_MAX_TEXTUER_SWITCH(idx)
 if(frag_color.a < 0.001)
    discard;
}
