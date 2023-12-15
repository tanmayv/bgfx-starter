$input a_position, a_texcoord0
$output v_uv

#include "bgfx_shader.sh"

void main()
{
	v_uv = a_texcoord0;
    mat4 projViewWorld = mul(mul(u_proj, u_view), u_model[0]);
    vec4 v_pos = mul(projViewWorld, vec4(a_position, 1.0));
    gl_Position = v_pos;
} 