#version 330 core

in vec2 FragTexCoord;
in vec3 FragNormal;
in vec3 FragPosition;

uniform sampler2D BaseColorTexture;
uniform sampler2D MetallicRoughnessTexture;
uniform sampler2D NormalMapTexture;

uniform vec3 LightDirection;

out vec4 FragColor;

void main() {
    // Sample base color texture
    vec3 baseColor = texture(BaseColorTexture, FragTexCoord).rgb;

    // Sample metallic and roughness values
    vec2 metallicRoughness = texture(MetallicRoughnessTexture, FragTexCoord).rg;

    // Sample and transform the normal map (from [0,1] to [-1,1])
    vec3 normalMap = texture(NormalMapTexture, FragTexCoord).rgb;
    vec3 normal = normalize(normalMap * 2.0 - 1.0);

    // Compute lighting (dot product between normal and light direction)
    float lightIntensity = max(dot(normal, -LightDirection), 0.0);

    // Separate metallic and roughness components
    float metallic = metallicRoughness.r;
    float roughness = metallicRoughness.g;

    // Combine lighting and material properties
    vec3 color = baseColor * lightIntensity * (1.0 - metallic) + baseColor * metallic;

    // Output final fragment color
    FragColor = vec4(color, 1.0);
}
