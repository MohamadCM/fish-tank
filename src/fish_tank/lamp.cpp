//
// Created by Mohamad Chamanmotlagh on 11/26/2024.
//
#include "lamp.h"
#include <shaders/advanced_material_vert_glsl.h>
#include <shaders/advanced_material_frag_glsl.h>



#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Lamp::mesh;
std::unique_ptr<ppgso::Shader> Lamp::shader;
// std::unique_ptr<ppgso::Texture> Lamp::baseColor;
// std::unique_ptr<ppgso::Texture> Lamp::metallicRoughness;
// std::unique_ptr<ppgso::Texture> Lamp::normalMap;
std::unique_ptr<ppgso::Texture> Lamp::texture;

Lamp::Lamp() {
    // Initialize static resources if needed
    // if (!shader) shader = std::make_unique<ppgso::Shader>(advanced_material_vert_glsl, advanced_material_frag_glsl);
    // if (!baseColor) baseColor = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/desk-light_baseColor.bmp"));
    // if (!metallicRoughness) metallicRoughness = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/desk-light_metallicRoughness.bmp"));
    // if (!normalMap) normalMap = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/desk-light_normal.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("lamp.gltf");

    //
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("wood.bmp"));


    scale = glm::vec3(0.04f, 0.04f, 0.04f);;
    rotation.x = glm::radians(-45.0f);
    rotation.z = glm::radians(45.0f);
    rotation.y = glm::radians(45.0f);
}

bool Lamp::update(Scene &scene, float dt) {
    // Lamp is static; no updates needed
    generateModelMatrix();
    return true;
}

void Lamp::render(Scene &scene) {
    // Use the shader
    shader->use();

    // Set up light direction
    shader->setUniform("LightDirection", scene.lightDirection);

    // Set up camera matrices
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // Set the model transformation matrix
    shader->setUniform("ModelMatrix", modelMatrix);

    // shader->setUniform("BaseColorTexture", *baseColor);
    // shader->setUniform("MetallicRoughnessTexture", *metallicRoughness);
    // shader->setUniform("NormalMapTexture", *normalMap);

    shader->setUniform("Texture", *texture);

    // Render the mesh
    mesh->render();
}
