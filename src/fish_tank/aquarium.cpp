//
// Created by Mcham on 11/26/2024.
//

#include "aquarium.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Aquarium::mesh;
std::unique_ptr<ppgso::Shader> Aquarium::shader;
std::vector<std::unique_ptr<ppgso::Texture>> Aquarium::textures;

Aquarium::Aquarium() {
    // Load shared resources if not already loaded
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);

    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("acquarium.gltf"); // Ensure the .gltf file is in the correct path

    if (textures.empty()) {
        textures.push_back(std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Aquarium_baseColor.bmp")));
        // textures.push_back(std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Aquarium_emissive.bmp")));
        // textures.push_back(std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Aquarium_metallicRoughness.bmp")));
        // textures.push_back(std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Dirt_baseColor.bmp")));
        // textures.push_back(std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Dirt_metallicRoughness.bmp")));
        // textures.push_back(std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Dirt_normal.bmp")));
    }
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
    shader->setUniform("BaseColorTexture", *textures[0]);
    // Bind additional textures as needed (e.g., normal map, metallic map)

    // Render the mesh
    mesh->render();
}
