#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

uniform mat4 mvp;

out vec3 worldPos;
out vec2 uv;

void main()
{
    worldPos = vertexPosition;
    uv = vertexTexCoord;

    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
