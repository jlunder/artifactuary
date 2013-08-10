precision lowp float;

varying vec4 v_color;
varying vec2 v_texCoord;

void main(void)
{
	gl_FragColor = vec4(v_color.x, v_color.y, v_color.z, 1.0);
}
