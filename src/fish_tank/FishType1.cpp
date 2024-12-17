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
    rotation = glm::ballRand(ppgso::PI);
    rotMomentum = glm::ballRand(ppgso::PI);

    speed = {glm::linearRand(-0.05f, 0.05f), glm::linearRand(-1.0f, 2.0f), glm::linearRand(-25.0f, 25.0f)};
    center = position;
}

bool FishType1::update(Scene& scene, float dt)
{
    shader->use();
    angle += velocity * dt;

    // Keep the angle within 0 to 2π for stability
    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }

    // Update the position using the parametric equation
    position.x = center.x + radius * cos(angle);
    position.y = center.y + radius * sin(angle);
    position.z = center.z + sin(angle);

    // Rotate the object

    rotation.x += rotMomentum.x * dt * 0.1f;
    rotation.y += rotMomentum.y * dt * 0.1f;
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

void FishType1::fleeFrom(const glm::vec3& predatorPosition, float fleeSpeed, float dt) {
    glm::vec3 direction = position - predatorPosition;
    float distance = glm::length(direction);

    if (distance < 5.0f) { // Flee when the predator is within a certain distance
        direction = glm::normalize(direction);
        position += direction * fleeSpeed * dt;
    }
}
