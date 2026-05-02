#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

uniform mat4 mvp;
uniform mat4 matModel;

out vec3 worldPos;
out vec2 uv;

void main()
{
  
  vec4 world = matModel * vec4(vertexPosition, 1.0);
  worldPos = world.xyz;
  uv = vertexTexCoord;

  gl_Position = mvp * vec4(vertexPosition, 1.0);
}
