#include "scene.h"
#include "table.h"
#include "bubble.h"

void Scene::update(float time)
{
    // Handle camera transition during a scene change
    if (transitionToNextScene)
    {
        transitionProgress += time * 0.5f; // Adjust the speed of the transition as needed

        // Limit the transition progress to 1.0 (2 seconds)
        if (transitionProgress > 1.0f)
        {
            transitionProgress = 1.0f;
        }

        // Interpolate the camera's position between initial and target positions
        camera->position = glm::mix(initialCameraPosition, targetCameraPosition, transitionProgress);
        camera->back = glm::normalize(camera->position - glm::vec3{-3.5f, 2.0f, -2.0f});
        // TODO: Hardcoded aquarium position
        camera->update();

        // If the transition is complete and next scene hasn't been triggered yet
        if (transitionProgress >= 1.0f && !nextSceneTriggered)
        {
            switchToNextScene();
            nextSceneTriggered = true;
            sceneIndex++;
        }
    }

    camera->update();

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);
    while (i != std::end(objects))
    {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(*this, time))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }

    if (sceneIndex == 1)
    {
        bubbleTimer += time;
        if (bubbleTimer > glm::linearRand(0.5f, 5.0f))
        {
            auto bubble = std::make_unique<Bubble>();
            bubble->position = glm::vec3(glm::linearRand(-15.0f, 15.0f), glm::linearRand(-10.0f, 10.0f),
                                         glm::linearRand(-20.0f, 20.0f));
            objects.push_back(std::move(bubble));
            bubbleTimer = 0.0f;
        }
    }
}

void Scene::render()
{
    // Simply render all objects
    for (auto& obj : objects)
        obj->render(*this);
}

std::vector<Object*> Scene::intersect(const glm::vec3& position, const glm::vec3& direction)
{
    std::vector<Object*> intersected = {};
    for (auto& object : objects)
    {
        // Collision with sphere of size object->scale.x
        auto oc = position - object->position;
        auto radius = object->scale.x;
        auto a = glm::dot(direction, direction);
        auto b = glm::dot(oc, direction);
        auto c = glm::dot(oc, oc) - radius * radius;
        auto dis = b * b - a * c;

        if (dis > 0)
        {
            auto e = sqrt(dis);
            auto t = (-b - e) / a;

            if (t > 0)
            {
                intersected.push_back(object.get());
                continue;
            }

            t = (-b + e) / a;

            if (t > 0)
            {
                intersected.push_back(object.get());
                continue;
            }
        }
    }

    return intersected;
}

void Scene::switchToNextScene()
{
    // Clear current scene objects
    objects.clear();

    // Reset transition variables
    transitionToNextScene = false;
    nextSceneTriggered = false;
    transitionProgress = 0.0f;
}
