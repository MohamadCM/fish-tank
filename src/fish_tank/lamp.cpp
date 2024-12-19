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
std::unique_ptr<ppgso::Texture> Lamp::baseColor;
std::unique_ptr<ppgso::Texture> Lamp::metallicRoughness;
std::unique_ptr<ppgso::Texture> Lamp::normalMap;

Lamp::Lamp()
{
    if (!shader) shader = std::make_unique<ppgso::Shader>(advanced_material_vert_glsl, advanced_material_frag_glsl);
    if (!baseColor) baseColor = std::make_unique<ppgso::Texture>(
        ppgso::image::loadBMP("textures/desk-light_baseColor.bmp"));
    if (!metallicRoughness) metallicRoughness = std::make_unique<ppgso::Texture>(
        ppgso::image::loadBMP("textures/desk-light_metallicRoughness.bmp"));
    if (!normalMap) normalMap = std::make_unique<ppgso::Texture>(
        ppgso::image::loadBMP("textures/desk-light_normal.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("lamp.gltf");

    scale = glm::vec3(0.04f, 0.04f, 0.04f);;
    rotation.x = glm::radians(-45.0f);
    rotation.z = glm::radians(45.0f);
    rotation.y = glm::radians(45.0f);
}

bool Lamp::update(Scene& scene, float dt)
{
    // Accumulate time
    elapsedTime += dt;

    // Oscillate position on the X-axis using sine wave
    position.x = -8.0f + sin(elapsedTime * 2.0f) * 1.5f;

    // Update the light position (top of the lamp)
    lightPosition = position + glm::vec3(scale.x * 5.0f, -scale.y * 5.0f, scale.z * 5.0f);
    // Adjust Y-offset as per lamp's height.

    // Add this lamp's light source to the scene
    scene.lightSources.push_back(lightPosition);

    // Generate model matrix
    generateModelMatrix();
    return true;
}

void Lamp::render(Scene& scene)
{
    // Use the shader
    shader->use();

    // Set up light direction
    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("LightPosition", scene.lightSources.back());
    shader->setUniform("LightColor", glm::vec3(1.0f, 1.0f, 0.9f)); // Warm light color
    // Set up camera matrices
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // Set the model transformation matrix
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("UseSimpleTexture", false);

    shader->setUniform("MetallicRoughnessTexture", *metallicRoughness);
    shader->setUniform("NormalMapTexture", *normalMap);
    shader->setUniform("BaseColorTexture", *baseColor);

    // Render the mesh
    mesh->render();
}
