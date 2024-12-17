//
// Created by Mcham on 11/26/2024.
//

#include "FishType2.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> FishType2::mesh;
std::unique_ptr<ppgso::Shader> FishType2::shader;
std::unique_ptr<ppgso::Texture> FishType2::texture;

FishType2::FishType2()
{
    // Load shared resources if not already loaded
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fish_2.gltf");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/fish_2_baseColor.bmp"));
    scale = glm::vec3(0.05f, 0.05f, 0.05f);
    rotation = glm::ballRand(ppgso::PI);
    rotMomentum = glm::ballRand(ppgso::PI);

    speed = {glm::linearRand(-0.05f, 0.05f), glm::linearRand(-1.0f, 2.0f), glm::linearRand(-25.0f, 25.0f)};;
}

bool FishType2::update(Scene& scene, float dt)
{
    shader->use();
    position += speed * dt;

    if (position.x > 15.0f || position.x < -15.0f) speed.x = -speed.x;
    if (position.y > 30.0f || position.y < -15.0f) speed.y = -speed.y;
    if (position.z > 50.0f || position.z < -150.0f) speed.z = -speed.z;

    // Rotate the object
    rotation.x += rotMomentum.x * dt * 0.1f;
    rotation.y += rotMomentum.y * dt * 0.1f;

    // Check for collisions with other fish
    for (auto& obj : scene.objects)
    {
        auto otherFish = dynamic_cast<Object*>(obj.get());
        if (otherFish && otherFish != this)
        {
            if (checkCollision(*otherFish))
            {
                resolveCollision(*otherFish);
            }
        }
    }

    generateModelMatrix();
    return true;
}

void FishType2::render(Scene& scene)
{
    // Set light uniforms
    shader->setUniform("LightDirection", scene.lightDirection);

    // Set camera matrices
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("Texture", *texture);

    // Render the mesh
    mesh->render();
}

void FishType2::fleeFrom(const glm::vec3& predatorPosition, float fleeSpeed, float dt)
{
    glm::vec3 direction = position - predatorPosition;
    float distance = glm::length(direction);

    if (distance < 5.0f)
    {
        // Flee when the predator is within a certain distance
        direction = glm::normalize(direction);
        position += direction * fleeSpeed * dt;
    }
}

bool FishType2::checkCollision(Object& otherFish)
{
    float distance = glm::length(position - otherFish.position);
    return distance < (boundingRadius + otherFish.boundingRadius);
}

void FishType2::resolveCollision(Object& otherFish)
{
    // Compute the collision normal
    glm::vec3 collisionNormal = glm::normalize(position - otherFish.position);

    // Reflect velocities along the collision normal
    speed = glm::reflect(speed, collisionNormal);

    // Slightly separate the fish to prevent sticking
    position += collisionNormal * 0.01f;
    otherFish.position -= collisionNormal * 0.01f;
}
