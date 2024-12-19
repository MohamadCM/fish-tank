#include "scene.h"
#include "table.h"
#include "bubble.h"
#include "FishType1.h"
#include "FishType2.h"
#include "Shark.h"

void Scene::update(float time)
{
    if(sceneIndex == 0) // Move camera on a bezier curve on the first scene
    {
        cameraTime += cameraSpeed * time;
        if (cameraTime > 1.0f) cameraTime = 0.0f; // Loop back

        glm::vec3 newPosition = evaluateBezier(cameraTime, bezierP0, bezierP1, bezierP2, bezierP3);
        camera->position = newPosition;

        // Ensure the camera is always looking at the table
        glm::vec3 tableCenter = {-10.0f, -3.0f, -10.0f}; // Adjust based on table position
        camera->back = glm::normalize(camera->position - tableCenter);
    }

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

    std::vector<Object*> sharkList;
    std::vector<Object*> fishList;

    for (auto& obj : objects)
    {
        if (dynamic_cast<Shark*>(obj.get()))
        {
            auto shark = dynamic_cast<Shark*>(obj.get());
            if (!shark)
            {
                sharkList.push_back(shark);
            }
        }
        else if (dynamic_cast<FishType1*>(obj.get()))
        {
            fishList.push_back(obj.get());
        }
    }

    if (!sharkList.empty())
    {
        for (auto shark : sharkList)
        {
            // Shark chases the nearest fish
            Object* closestFish = nullptr;
            float minDistance = std::numeric_limits<float>::max();

            for (auto fish : fishList)
            {
                float distance = glm::length(shark->position - fish->position);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestFish = fish;
                }
            }

            if (closestFish)
            {
                (dynamic_cast<Shark*>(shark))->chase(closestFish->position, 2.0f, time);
                // Shark moves at chaseSpeed = 2.0f
            }

            // Fish flee from the shark
            for (auto fish : fishList)
            {
                if (shark)
                {
                    (dynamic_cast<FishType1*>(fish))->fleeFrom(shark->position, 3.0f, time);
                    // Fish move at fleeSpeed = 3.0f
                }
            }
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

glm::vec3 Scene::evaluateBezier(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
                                const glm::vec3& p3)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec3 point = (uuu * p0) + (3 * uu * t * p1) + (3 * u * tt * p2) + (ttt * p3);
    return point;
}
