#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main()
{
    vec3 norm = normalize(fragNormal);
    vec3 lightDirNorm = normalize(lightDir);
    float diff = max(dot(norm, -lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = ambientColor;
    vec4 texColor = texture(texture0, fragTexCoord);
    finalColor = vec4((ambient + diffuse) * texColor.rgb, texColor.a);
}