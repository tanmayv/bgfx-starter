$input a_position, a_texcoord0
$output v_uv

#include "bgfx_shader.sh"
uniform mat4 u_transform;
uniform mat4 u_view1;
uniform mat4 u_model1;
uniform mat4 u_projection1;

void main()
{
	v_uv = a_texcoord0;
    mat4 projViewWorld = mul(mul(u_projection1, u_view1), u_model1);
    vec4 v_pos = mul(projViewWorld, vec4(a_position, 1.0));
    gl_Position = v_pos;
} 