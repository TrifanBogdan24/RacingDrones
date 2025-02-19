#version 330 core

out vec4 outColor;

uniform vec3 color1;
uniform vec3 color2;


void main()
{
    outColor = vec4(mix(color1, color2, 0.4), 1.0);
}
