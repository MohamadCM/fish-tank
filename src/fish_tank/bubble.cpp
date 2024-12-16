//
// Created by Mcham on 11/26/2024.
//

#include "Bubble.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Bubble::mesh;
std::unique_ptr<ppgso::Shader> Bubble::shader;
std::unique_ptr<ppgso::Texture> Bubble::texture;

Bubble::Bubble()
{
    // Load shared resources if not already loaded
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/ocean.bmp"));
    lifetime = glm::linearRand(1.0f, 6.0f);
    age = 0.0f;

    scale *= glm::linearRand(0.1f, 0.5f);
    speed = {0.0, glm::linearRand(-3.0f, 3.0f), glm::linearRand(-0.5f, 0.5f)};
}

bool Bubble::update(Scene& scene, float dt)
{
    shader->use();
    position += speed * dt;
    age += dt;
    if (age > lifetime) return false;

    if (position.x > 15.0f || position.x < -15.0f) speed.x = -speed.x;
    if (position.y > 12.5f || position.y < -12.5f) speed.y = -speed.y;
    if (position.z > 15.0f || position.z < -15.0f) speed.z = -speed.z;

    // Rotate the object
    rotation += rotMomentum * dt;

    generateModelMatrix();
    return true;
}

void Bubble::render(Scene& scene)
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
