attribute highp vec3 posin;

uniform highp mat4 u_pv;
uniform highp mat4 u_trans;

void main()
{
  gl_Position = u_pv * u_trans * vec4(posin, 1.0);
}
