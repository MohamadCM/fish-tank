#version 330

// Vertex attributes
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Matrices for transformations
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Output to fragment shader
out vec2 texCoord;        // Texture coordinates
out vec3 FragPosition;    // World-space fragment position
out vec3 FragNormal;      // World-space normal

void main() {
  // Pass texture coordinates directly
  texCoord = TexCoord;

  // Calculate world-space position of the fragment
  vec4 worldPosition = ModelMatrix * vec4(Position, 1.0);
  FragPosition = worldPosition.xyz;

  // Calculate world-space normal
  FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;

  // Final vertex position in clip space
  gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
}
