#version 330

in vec2 uv;

out vec4 finalColor;

void main()
{
    float h = uv.y;

    vec3 color = mix(
        vec3(0.2, 0.6, 0.2),
        vec3(0.7, 0.7, 0.7),
        h
    );

    finalColor = vec4(color, 1.0);
}
