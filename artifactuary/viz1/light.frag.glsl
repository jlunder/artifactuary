precision lowp float;

varying vec4 v_color;
varying vec2 v_texCoord;

void main(void)
{
	const vec2 middle = vec2(0.5, 0.5);
	gl_FragColor = vec4(1.0, 1.0, 1.0, max(0.0, 1.0 - 2.0 * distance(middle, v_texCoord)));
}
