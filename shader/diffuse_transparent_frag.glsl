#version 330

// Uniforms
uniform sampler2D Texture;
uniform vec3 LightDirection;
uniform vec3 LightPosition; // Point light position
uniform vec3 LightColor;    // Point light color
uniform vec3 CameraPosition; // Camera position for specular calculations
uniform float Transparency;
uniform vec2 TextureOffset;

// Inputs from vertex shader
in vec2 texCoord;
in vec3 FragPosition;
in vec3 FragNormal;

// Output color
out vec4 FragmentColor;

void main() {
  // Normalize the inputs
  vec3 norm = normalize(FragNormal);

  // Compute diffuse lighting for directional light
  float diffuseDir = max(dot(norm, -normalize(LightDirection)), 0.0);

  // Compute point light contribution
  vec3 lightDir = normalize(LightPosition - FragPosition);
  float distance = length(LightPosition - FragPosition);
  float attenuation = 1.0 / (distance * distance);
  float diffusePoint = max(dot(norm, lightDir), 0.0) * attenuation;

  // Compute specular reflection (Blinn-Phong)
  vec3 viewDir = normalize(CameraPosition - FragPosition);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float specular = pow(max(dot(norm, halfwayDir), 0.0), 32.0) * attenuation;

  // Sample the texture color
  vec4 textureColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

  // Combine lighting contributions
  vec3 lighting = textureColor.rgb * (diffuseDir + diffusePoint * LightColor) + vec3(specular);

  // Apply the transparency uniform to the alpha value
  float finalAlpha = textureColor.a * Transparency;

  // Output the final color, including alpha
  FragmentColor = vec4(lighting, finalAlpha);
}
