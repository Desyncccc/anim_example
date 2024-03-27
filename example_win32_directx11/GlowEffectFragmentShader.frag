#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float glowIntensity; // Control the intensity of the glow
uniform vec3 glowColor; // Color of the glow

void main()
{
    vec4 texColor = texture(texture1, TexCoord);

    // Calculate glow factor based on distance from the center (for a circular object)
    float dist = distance(TexCoord, vec2(0.5, 0.5));
    float glowFactor = 1.0 - smoothstep(0.4, 0.5, dist); // Adjust these values as needed

    // Apply glow color and intensity
    vec3 glow = glowColor * glowIntensity * glowFactor;

    FragColor = vec4(texColor.rgb + glow, texColor.a);
}
