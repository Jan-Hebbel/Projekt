#version 430 core

in vec2 texCoord;

out vec4 FragColor;

layout(location = 0) uniform sampler2D textureAtlas;

void main()
{
	FragColor = texture(textureAtlas, texCoord);
}