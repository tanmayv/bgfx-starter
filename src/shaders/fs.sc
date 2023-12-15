$input v_uv

#include "bgfx_shader.sh"

SAMPLER2D(s_texColor,  0);

void main()
{
    vec4 color = texture2D(s_texColor, v_uv);
	gl_FragColor = vec4(1.0, 0.5, 1.0, 1.0);
}