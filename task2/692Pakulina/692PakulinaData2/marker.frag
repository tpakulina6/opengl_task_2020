/**
Простой шейдер для отрисовки маркера источника света
*/

#version 330

uniform vec4 color;

out vec4 fragColor; //выходной цвет фрагмента

void main()
{
	fragColor = vec4(1, 1, 0.5, 1);
}
