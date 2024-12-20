#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "RoomBackground.h"
#include "table.h"
#include "camera.h"
#include "scene.h"
#include "lamp.h"
#include "aquarium.h"
#include "asteroid.h"
#include "BezierSurface.h"
#include "FishType1.h"
#include "FishType2.h"
#include "WaterBackground.h"
#include "bubble.h"
#include "Shark.h"
#define NUMBER_OF_FISH_1 20
#define NUMBER_OF_FISH_2 15
#define NUMBER_OF_SHARK 5

const unsigned int SIZE = 768;

/*!
 * Custom window for our simple game
 */
class SceneWindow : public ppgso::Window
{
private:
    Scene scene;

    // Camera movement speed
    float cameraSpeed = 2.0f;

    // Camera movement state
    glm::vec3 cameraMovement = {0.0f, 0.0f, 0.0f};
    /*!
     * Reset and initialize the first scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene()
    {
        scene.objects.clear();

        // Light Direction
        scene.lightDirection = {-20.0f, 0.0f, 1.5f};

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = {10.0f, -10.0f, 10.0f}; // Set the camera position
        camera->back = glm::normalize(camera->position - glm::vec3{0.0f, 0.0f, 0.0f});
        camera->update(); // Ensure the view matrix is updated
        scene.camera = std::move(camera);

        scene.transitionToNextScene = false;
        scene.transitionProgress = 0.0f;
        scene.nextSceneTriggered = false;
    }

    // Implementation of the first scene objects
    void createFirstScene()
    {
        // Add room background
        // auto background = std::make_unique<RoomBackground>();
        // scene.objects.push_back(std::move(background));

        // Add lamp to the scene
        auto lamp = std::make_unique<Lamp>();
        lamp->position = {-8.0f, 1.0f, -3.0f};
        scene.objects.push_back(std::move(lamp));

        // Add table to the scene
        auto table = std::make_unique<Table>();
        table->position = {-10.0f, -3.0f, -10.0f};
        table->rotation.z = glm::radians(45.0f);
        table->rotation.x = glm::radians(-45.0f);
        scene.objects.push_back(std::move(table));

        // Add aquarium to the scene
        auto aquarium = std::make_unique<Aquarium>(scene.objects.back().get());
        aquarium->rotation.z = glm::radians(45.0f);
        aquarium->rotation.x = glm::radians(-45.0f);
        aquarium->rotation.y = glm::radians(-90.0f);
        aquarium->offset = {6.5f, 5.0f, 8.0f};
        scene.objects.push_back(std::move(aquarium));

        // Initialize camera transition variables
        scene.initialCameraPosition = scene.camera->position; // Starting position
        // {10.0f, -10.0f, 10.0f}
        scene.targetCameraPosition = {0.5f, 0.0f, -0.0f}; // Near the aquarium

        scene.bezierP0 = {-20.0f, 10.0f, 15.0f};  // Start point (closer to the table)
        scene.bezierP1 = {-10.0f, 15.0f, 25.0f};  // Control point 1 (slightly to the left and closer)
        scene.bezierP2 = {10.0f, 15.0f, 25.0f};   // Control point 2 (slightly to the right and closer)
        scene.bezierP3 = {20.0f, 10.0f, 15.0f};   // End point (closer to the table)
    }

    // Implementation of the second scene objects
    void createSecondScene()
    {
        initScene();
        // Add room background
        auto background = std::make_unique<WaterBackground>();
        scene.objects.push_back(std::move(background));

        for (int i = 0; i <= NUMBER_OF_FISH_1; i++)
        {
            auto fish = std::make_unique<FishType1>();
            fish->position = {
                glm::linearRand(-5.0f, 5.0f), glm::linearRand(-5.0f, 5.0f), glm::linearRand(-20.0f, 40.0f)
            };
            fish->center = fish->position;
            fish->velocity = glm::linearRand(0.0f, 10.0f) * 0.1f;
            fish->radius = glm::linearRand(0.0f, 15.0f) * 3.0f;
            scene.objects.push_back(std::move(fish));
        }
        for (int i = 0; i <= NUMBER_OF_FISH_2; i++)
        {
            auto fish2 = std::make_unique<FishType2>();
            fish2->position = {
                glm::linearRand(-5.0f, 5.0f), glm::linearRand(-5.0f, 5.0f), glm::linearRand(-20.0f, 40.0f)
            };
            scene.objects.push_back(std::move(fish2));
        }

        for (int i = 0; i <= NUMBER_OF_SHARK; i++)
        {
            auto shark = std::make_unique<Shark>(true);
            shark->position = {
                glm::linearRand(-5.0f, 5.0f), glm::linearRand(-5.0f, 5.0f), glm::linearRand(-20.0f, 40.0f)
            };
            scene.objects.push_back(std::move(shark));
        }

        for (int i = 0; i < 5; i++)
        {
            auto bubble = std::make_unique<Bubble>();
            bubble->position = glm::vec3(glm::linearRand(-15.0f, 5.0f), glm::linearRand(-10.0f, 0.0f),
                                         glm::linearRand(-10.0f, 10.0f));
            scene.objects.push_back(std::move(bubble));
        }


        float groundMin = -10.0f;
        float groundMax = 10.0f;
        float groundHeight = -15.0f;
        spawnAsteroids(scene, 50, groundMin, groundMax, groundHeight);

        // auto ground = std::make_unique<BezierSurface>();
        // scene.objects.push_back(std::move(ground));
    }
    bool animate = true;

public:
    /*!
     * Construct custom scene window
     */
    SceneWindow() : Window{"gl9_scene", SIZE, SIZE}
    {
        // Hide cursor if needed
        // hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();

        createFirstScene();
        // createSecondScene();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override
    {
        scene.keyboard[key] = action;

        // Reset
        if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            initScene();
        }

        // Pause
        if (key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            animate = !animate;
        }

        // Start camera transition and switch scene
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !scene.transitionToNextScene)
        {
            scene.transitionToNextScene = true; // Start the transition
        }

        // Camera movement
        if (action == GLFW_PRESS || action == GLFW_RELEASE)
        {
            if (scene.sceneIndex >= 1)
            {
                float direction = (action == GLFW_PRESS) ? 1.0f : 0.0f;

                switch (key)
                {
                case GLFW_KEY_UP:
                    cameraMovement.z = -direction; // Move forward
                    break;
                case GLFW_KEY_DOWN:
                    cameraMovement.z = direction; // Move backward
                    break;
                case GLFW_KEY_LEFT:
                    cameraMovement.x = -direction; // Move left
                    break;
                case GLFW_KEY_RIGHT:
                    cameraMovement.x = direction; // Move right
                    break;
                }
            }
        }
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override
    {
        scene.cursor.x = cursorX;
        scene.cursor.y = cursorY;
    }

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse button state
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            scene.cursor.left = action == GLFW_PRESS;

            if (scene.cursor.left)
            {
                // Convert pixel coordinates to screen coordinates
                double u = (scene.cursor.x / width - 0.5f) * 2.0f;
                double v = -(scene.cursor.y / height - 0.5f) * 2.0f;

                // Get mouse pick vector in world coordinates
                auto direction = scene.camera->cast(u, v);
                auto position = scene.camera->position;

                // Get all objects in scene intersected by ray
                auto picked = scene.intersect(position, direction);

                // Go through all objects that have been picked
                for (auto& obj : picked)
                {
                    // Pass on the click event
                    obj->onClick(scene);
                }
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            scene.cursor.right = action == GLFW_PRESS;
        }
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override
    {
        // Track time
        static auto time = (float)glfwGetTime();

        // Compute time delta
        float dt = animate ? (float)glfwGetTime() - time : 0;

        time = (float)glfwGetTime();

        if (scene.camera)
        {
            glm::vec3 movement = cameraMovement * cameraSpeed * dt;

            // Move the camera in world space
            scene.camera->position += movement;

            // Recalculate the "back" vector to keep the camera looking at the same point
            scene.camera->back = glm::normalize(scene.camera->position - glm::vec3{0.0f, 0.0f, 0.0f});
            scene.camera->update();
        }
        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        if (scene.nextSceneTriggered)
        {
            createSecondScene();
        }

        scene.render();
    }

    void spawnAsteroids(Scene& scene, int count, float groundMin, float groundMax, float groundHeight) {
        for (int i = 0; i < count; ++i) {
            // Generate random position within ground bounds
            float x = glm::linearRand(groundMin, groundMax);
            float z = glm::linearRand(groundMin, groundMax);

            // Y-position slightly above the ground
            float y = groundHeight + glm::linearRand(0.5f, 2.0f);

            // Create an asteroid and set its position
            auto asteroid = std::make_unique<Asteroid>();
            asteroid->position = {x, y, z};

            // Add the asteroid to the scene
            scene.objects.push_back(std::move(asteroid));
        }
    }
};

int main()
{
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents())
    {
    }

    return EXIT_SUCCESS;
}