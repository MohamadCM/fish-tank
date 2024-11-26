#version 330 core

in vec2 FragTexCoord;
in vec3 FragNormal;
in vec3 FragPosition;

uniform sampler2D BaseColorTexture;
uniform sampler2D MetallicRoughnessTexture;
uniform sampler2D NormalMapTexture;

// Directional light
uniform vec3 LightDirection;

// Point light (lamp)
uniform vec3 LightPosition; // Lamp position
uniform vec3 LightColor;    // Color of the lamp light (e.g., white)

// Camera position for specular calculations
uniform vec3 CameraPosition;

out vec4 FragColor;

void main() {
    // Sample base color texture
    vec3 baseColor = texture(BaseColorTexture, FragTexCoord).rgb;

    // Sample metallic and roughness values
    vec2 metallicRoughness = texture(MetallicRoughnessTexture, FragTexCoord).rg;

    // Sample and transform the normal map (from [0,1] to [-1,1])
    vec3 normalMap = texture(NormalMapTexture, FragTexCoord).rgb;
    vec3 normal = normalize(normalMap * 2.0 - 1.0);

    // Compute lighting for the directional light (existing)
    float lightIntensityDir = max(dot(normal, -LightDirection), 0.0);

    // Calculate the direction for point light (lamp)
    vec3 lightDir = normalize(LightPosition - FragPosition);
    float distance = length(LightPosition - FragPosition); // Distance from the light to the fragment
    float attenuation = 1.0 / (distance * distance); // Simple attenuation by distance (inverse square law)
    float lightIntensityLamp = max(dot(normal, lightDir), 0.0) * attenuation;

    // Separate metallic and roughness components
    float metallic = metallicRoughness.r;
    float roughness = metallicRoughness.g;

    // Combine lighting from both the directional light and the lamp
    vec3 color = baseColor * (lightIntensityDir * (1.0 - metallic) + lightIntensityLamp * LightColor);

    // Output final fragment color
    FragColor = vec4(color, 1.0);
}
