#version 330 core

in vec2 fragTexCoord;

uniform sampler2D textureSampler;

out vec4 fragColor;

void main() {
    vec4 color = texture(textureSampler, fragTexCoord);

    // Convert to grayscale using luminance coefficients
    float grayscale = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
    fragColor = vec4(vec3(grayscale), color.a);
}
