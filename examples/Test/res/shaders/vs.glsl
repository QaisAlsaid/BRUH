#version 330 core
layout (location = 0) in vec3 posin;
out vec4 fcol;
void main()
{
  gl_Position = vec4(posin, 1.0);
  fcol = vec4(0.5, 0.4, 0.6, 1.0f);
}
