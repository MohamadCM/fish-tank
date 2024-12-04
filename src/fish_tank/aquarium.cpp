//
// Created by Mcham on 11/26/2024.
//

#include "aquarium.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Aquarium::mesh;
std::unique_ptr<ppgso::Shader> Aquarium::shader;
std::unique_ptr<ppgso::Texture> Aquarium::texture;

Aquarium::Aquarium() {
    // Load shared resources if not already loaded
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("aquarium.gltf");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/glass.bmp"));
    scale = glm::vec3(0.45f, 0.45f, 0.45f);
}

bool Aquarium::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Aquarium::render(Scene &scene) {
    shader->use();

    // Set light uniforms
    shader->setUniform("LightDirection", scene.lightDirection);

    // Set camera matrices
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);

    // Bind textures (example for base color)
  shader->setUniform("Texture", *texture);
    // Bind additional textures as needed (e.g., normal map, metallic map)

    // Render the mesh
    mesh->render();
}
