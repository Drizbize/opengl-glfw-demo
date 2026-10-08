#version 330 core

out vec4 FragColor;

uniform float time;

void main()
{
    float value = mod(time, 1.0);
    FragColor = vec4(value, value, value, 0.0);
}