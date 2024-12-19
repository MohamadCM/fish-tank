#version 330 core

in vec2 FragTexCoord;
in vec3 FragNormal;
in vec3 FragPosition;

// Base color texture
uniform sampler2D BaseColorTexture;

// Metallic and roughness texture
uniform sampler2D MetallicRoughnessTexture;

// Normal map texture
uniform sampler2D NormalMapTexture;

// Directional light properties
uniform vec3 LightDirection;

// Point light (lamp) properties
uniform vec3 LightPosition; // Position of the lamp
uniform vec3 LightColor;    // Color/intensity of the lamp light

// Camera position (for specular reflection)
uniform vec3 CameraPosition;

// Final output color
out vec4 FragColor;

void main() {
    // 1. Sample base color
    vec3 baseColor = texture(BaseColorTexture, FragTexCoord).rgb;

    // 2. Sample metallic and roughness values
    vec2 metallicRoughness = texture(MetallicRoughnessTexture, FragTexCoord).rg;
    float metallic = metallicRoughness.r;
    float roughness = clamp(metallicRoughness.g, 0.05, 1.0); // Avoid extreme smoothness for stability

    // 3. Sample and normalize normal map
    vec3 normalMap = texture(NormalMapTexture, FragTexCoord).rgb;
    vec3 normal = normalize(normalMap * 2.0 - 1.0); // Transform [0,1] range to [-1,1]

    // 4. Compute lighting for directional light
    vec3 lightDirDir = normalize(-LightDirection); // Ensure normalized light direction
    float lightIntensityDir = max(dot(normal, lightDirDir), 0.0);

    // 5. Compute lighting for point light
    vec3 lightDirPoint = normalize(LightPosition - FragPosition);
    float distance = length(LightPosition - FragPosition);
    float attenuation = 1.0 / (distance * distance); // Inverse square law for attenuation
    float lightIntensityPoint = max(dot(normal, lightDirPoint), 0.0) * attenuation;

    // 6. Calculate specular reflection using Blinn-Phong model
    vec3 viewDir = normalize(CameraPosition - FragPosition);
    vec3 halfwayDir = normalize(lightDirPoint + viewDir);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), 32.0 * (1.0 - roughness)) * attenuation;

    // 7. Combine lighting contributions
    vec3 diffuse = baseColor * (lightIntensityDir + lightIntensityPoint * LightColor);
    vec3 ambient = baseColor * 0.1; // Add a simple ambient term
    vec3 finalColor = ambient + diffuse + specular * metallic;


    // Post-Processing shaders
    vec4 color = texture(BaseColorTexture, FragTexCoord);
    float grayscale = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
    FragColor = vec4(vec3(grayscale), color.a);

    // 8. Output the final color
    // FragColor = vec4(finalColor, 1.0);
}
