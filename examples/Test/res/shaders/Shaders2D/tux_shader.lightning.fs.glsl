#version 330 core


in vec3 o_frag_pos;

uniform vec4 u_tint_color;
uniform vec2 u_light_pos;
uniform vec4 u_light_color;
out vec4 frag_color;

void main()
{

  float ambient_factor = 0.5;
  vec4 ambient = ambient_factor * u_light_color;
  vec3 light_dir = normalize(vec3(u_light_pos, 0.0) - o_frag_pos);
  //vec3(0,0,1) //should be view_pos and in view_dir also
  float diff = max(dot(vec3(0.0, 0.0, 1.0), light_dir), 0.0);
  vec4 diffuse = diff * u_light_color;
  float specular_factor = 2;
  vec3 view_dir = normalize(vec3(0.0, 0.0, 1.0) - o_frag_pos);
  vec3 reflect_dir = reflect(-light_dir, o_frag_pos);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 0.2);
  vec4 specular = specular_factor * spec * u_light_color;
  vec4 resault = ambient + diffuse + specular;
  frag_color =  u_tint_color * 2.0/resault;
}
