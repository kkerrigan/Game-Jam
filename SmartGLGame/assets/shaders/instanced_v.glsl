#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 instanceMatrix; //this is 4 x vec4
layout (location = 5) in vec4 color;

out vec4 fColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    fColor = color;
}  