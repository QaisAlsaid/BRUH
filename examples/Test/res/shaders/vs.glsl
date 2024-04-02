#version 330 core
layout (location = 0) in vec3 posin;
uniform mat4 u_pv;
void main()
{
  gl_Position = u_pv * vec4(posin, 1.0);
}
