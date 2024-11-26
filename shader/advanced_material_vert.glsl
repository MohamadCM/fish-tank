#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 FragTexCoord;
out vec3 FragNormal;
out vec3 FragPosition;

void main() {
    // Pass texture coordinates to fragment shader
    FragTexCoord = TexCoord;

    // Transform normals to world space
    FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;

    // Calculate position in world space
    FragPosition = vec3(ModelMatrix * vec4(Position, 1.0));

    // Calculate final vertex position in clip space
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPosition, 1.0);
}
