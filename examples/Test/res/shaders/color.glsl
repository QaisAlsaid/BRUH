#KAREN_SHADER_TYPE KAREN_VERTEX
#version 330 core
layout (location = 0) in vec3 posin;
uniform mat4 u_pv;
uniform mat4 u_trans;
void main()
{
  gl_Position = u_pv * u_trans * vec4(posin, 1.0);
}

#KAREN_SHADER_TYPE KAREN_FRAGMENT
#version 330 core
out vec4 frag_color;
void main()
{
  frag_color = vec4(1.0, 0.3, 0.1, 1.0);
}
