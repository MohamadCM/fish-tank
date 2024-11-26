const char* advanced_material_frag_glsl = R"(
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
    // Sample textures
    vec3 baseColor = texture(BaseColorTexture, FragTexCoord).rgb;
    vec2 metallicRoughness = texture(MetallicRoughnessTexture, FragTexCoord).rg;
    vec3 normalMap = texture(NormalMapTexture, FragTexCoord).rgb;

    // Transform normal map (convert from [0,1] to [-1,1])
    vec3 normal = normalize(normalMap * 2.0 - 1.0);

    // Calculate lighting
    float lightIntensity = max(dot(normal, -LightDirection), 0.0);

    // Calculate roughness and metallic properties
    float roughness = metallicRoughness.g;
    float metallic = metallicRoughness.r;

    // Combine into final color
    vec3 color = baseColor * lightIntensity * (1.0 - metallic) + baseColor * metallic;

    FragColor = vec4(color, 1.0);
}
)";
