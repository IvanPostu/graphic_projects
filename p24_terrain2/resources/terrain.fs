#version 330

in vec3 fragPos;
out vec4 finalColor;

void main()
{
    float h = fragPos.y;

    if (h < 2.0)
        finalColor = vec4(0.2, 0.6, 0.2, 1.0); // grass
    else if (h < 5.0)
        finalColor = vec4(0.5, 0.4, 0.2, 1.0); // dirt
    else
        finalColor = vec4(0.8, 0.8, 0.8, 1.0); // rock/snow
}
