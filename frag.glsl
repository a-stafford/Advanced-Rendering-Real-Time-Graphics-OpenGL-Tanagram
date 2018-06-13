#version 330 core
out vec4 color;
in vec3 My_color;
void main()
{
color = vec4(My_color, 1.0f);
}