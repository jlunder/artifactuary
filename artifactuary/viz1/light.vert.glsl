uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_modelMatrix;

attribute vec4 a_vertex;
attribute vec4 a_color;
attribute vec2 a_texCoord;

varying vec4 v_color;
varying vec2 v_texCoord;

void main(void)
{
	gl_Position = a_vertex; //u_viewProjectionMatrix * u_modelMatrix * a_vertex;
	v_color = a_color;
	v_texCoord = a_texCoord;
}
