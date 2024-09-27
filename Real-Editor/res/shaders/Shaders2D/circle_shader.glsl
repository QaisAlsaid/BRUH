#REAL_SHADER_TYPE REAL_VERTEX
#version 330 core

layout (location = 0) in vec4 a_color;
layout (location = 1) in vec3 a_pos;
layout (location = 2) in vec2 a_local_pos;
layout (location = 3) in float a_thickness;
layout (location = 4) in float a_blur;
//layout (location = 5) in int i_id;

uniform mat4 u_proj_view;
uniform mat4 u_trans;

out vec4 v_color;
out vec2 v_local_pos;
out float v_thickness;
out float v_blur;

//out int v_id;

void main()
{
  gl_Position = u_proj_view * vec4(a_pos, 1.0);
  v_color = a_color;
  v_local_pos = a_local_pos;
  v_thickness = a_thickness;
  v_blur = a_blur;
  //v_id = a_id;
}

#REAL_SHADER_TYPE REAL_FRAGMENT

#version 330 core

layout (location = 0) out vec4 frag_color;
layout (location = 1) out int e_id;

in vec4 v_color;
in vec2 v_local_pos;
in float v_thickness;
in float v_blur;
//in int v_id;



void main()
{
  e_id = 1;
  float dest = 1.0 - length(v_local_pos);
  float alpha = smoothstep(0, v_blur, dest);
  alpha *= smoothstep(v_thickness + v_blur, v_thickness, dest);
  
  frag_color = v_color;
  frag_color.a *= alpha;
  if(frag_color.a < 0.001)
    discard;
}
