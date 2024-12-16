//
// Created by Mcham on 11/26/2024.
//

#include "FishType1.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> FishType1::mesh;
std::unique_ptr<ppgso::Shader> FishType1::shader;
std::unique_ptr<ppgso::Texture> FishType1::texture;

FishType1::FishType1()
{
    // Load shared resources if not already loaded
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fish_1.gltf");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/fish_1_baseColor.bmp"));
    scale = glm::vec3(5.0f, 5.0f, 5.0f);
    // rotation = glm::ballRand(ppgso::PI);
    // rotMomentum = glm::ballRand(ppgso::PI);

    speed = {glm::linearRand(-2.0f, 2.0f), glm::linearRand(-5.0f, -10.0f), 3.0f};
}

bool FishType1::update(Scene& scene, float dt)
{
    shader->use();
    position += speed * dt;

    if (position.x > 15.0f || position.x < -15.0f) speed.x = -speed.x;
    if (position.y > 12.5f || position.y < -12.5f) speed.y = -speed.y;
    if (position.z > 15.0f || position.z < -15.0f) speed.z = -speed.z;

    // Rotate the object
    rotation += rotMomentum * dt;

    generateModelMatrix();
    return true;
}

void FishType1::render(Scene& scene)
{
    // Set light uniforms
    shader->setUniform("LightDirection", scene.lightDirection);
    // shader->setUniform("LightPosition", scene.lightSources.back());

    // Set camera matrices
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("Texture", *texture);

    // Render the mesh
    mesh->render();
}
