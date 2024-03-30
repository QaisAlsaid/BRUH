#version 330 core
layout (location = 0) in vec3 posin;
layout (location = 1) in vec3 colorin;
out vec4 fcol;
void main()
{
  gl_Position = vec4(posin, 1.0);
  fcol = vec4(colorin, 1.0f);
}