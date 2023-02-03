#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoord;

layout(location = 0) uniform mat4 mvp;

void main()
{
	texCoord = aTexCoord;
	gl_Position = mvp * vec4(aPosition, 1.0);
}