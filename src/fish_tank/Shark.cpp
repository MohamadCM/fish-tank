//
// Created by Mcham on 12/07/2024.
//

#include "Shark.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Shark::mesh;
std::unique_ptr<ppgso::Shader> Shark::shader;
std::unique_ptr<ppgso::Texture> Shark::texture;

Shark::Shark (bool keyframeAnimationActivated)
{
    // Load shared resources if not already loaded
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("shark.gltf");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/shark.bmp"));
    scale = glm::vec3(10.0f, 10.0f, 10.0f);
    rotation = glm::ballRand(ppgso::PI);
    rotMomentum = glm::ballRand(ppgso::PI);

    speed = {glm::linearRand(-0.05f, 0.05f), glm::linearRand(-1.0f, 2.0f), glm::linearRand(-25.0f, 25.0f)};

        generateKeyframes();

        // Initialize shark position and rotation
        currentPosition = keyframes.front().position;
        currentRotation = keyframes.front().rotation;

    // Set up VAO, VBO, etc. (model loading skipped here)
}

bool Shark::update(Scene& scene, float dt)
{
    // if (keyframeAnimationActivated)
    // {
        updateAnimation(dt);
    // }
    // else
    // {
    //     position += speed * dt;
    //
    //     if (position.x > 15.0f || position.x < -15.0f) speed.x = -speed.x;
    //     if (position.y > 30.0f || position.y < -15.0f) speed.y = -speed.y;
    //     if (position.z > 50.0f || position.z < -150.0f) speed.z = -speed.z;
    //
    //     rotation.x += rotMomentum.x * dt * 0.1f;
    //     rotation.y += rotMomentum.y * dt * 0.1f;
    // }

    shader->use();
    generateModelMatrix();
    return true;
}

void Shark::render(Scene& scene)
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

void Shark::chase(const glm::vec3& preyPosition, float chaseSpeed, float dt)
{
    glm::vec3 direction = preyPosition - position;
    float distance = glm::length(direction);

    if (distance > 0.5f)
    {
        // Stop if close enough to the prey
        direction = glm::normalize(direction);
        position += direction * chaseSpeed * dt;
    }
}

void Shark::generateKeyframes()
{
    // Define a circular animation path
    keyframes = {
        {{-10.0f, 5.0f, -10.0f}, {0.0f, 0.0f, 0.0f}, 0.0f},
        {{0.0f, 6.0f, -5.0f}, {0.0f, glm::radians(45.0f), 0.0f}, 2.0f},
        {{10.0f, 4.0f, 0.0f}, {0.0f, glm::radians(90.0f), 0.0f}, 4.0f},
        {{5.0f, 3.0f, 5.0f}, {0.0f, glm::radians(135.0f), 0.0f}, 6.0f},
        {{-5.0f, 5.0f, 10.0f}, {0.0f, glm::radians(180.0f), 0.0f}, 8.0f},
        {{-10.0f, 5.0f, -10.0f}, {0.0f, glm::radians(360.0f), 0.0f}, 10.0f} // Back to start
    };
}

void Shark::updateAnimation(float dt)
{
    elapsedTime += dt;

    // Loop animation
    float totalTime = keyframes.back().time;
    elapsedTime = fmod(elapsedTime, totalTime);

    // Find the two keyframes to interpolate between
    Keyframe* k1 = nullptr;
    Keyframe* k2 = nullptr;

    for (size_t i = 0; i < keyframes.size() - 1; ++i)
    {
        if (elapsedTime >= keyframes[i].time && elapsedTime < keyframes[i + 1].time)
        {
            k1 = &keyframes[i];
            k2 = &keyframes[i + 1];
            break;
        }
    }

    if (!k1 || !k2) return;

    // Calculate interpolation factor
    float t = (elapsedTime - k1->time) / (k2->time - k1->time);
    currentPosition = glm::mix(k1->position, k2->position, t);
    currentRotation = glm::mix(k1->rotation, k2->rotation, t);

    position = currentPosition;
    rotation = currentRotation;
}
