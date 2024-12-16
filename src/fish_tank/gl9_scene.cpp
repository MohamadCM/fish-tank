// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

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

const unsigned int SIZE = 768;

/*!
 * Custom window for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;

    /*!
     * Reset and initialize the first scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();

        // Light Direction
        scene.lightDirection = {-20.0f, 0.0f, 1.5f};

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = {10.0f, -10.0f, 10.0f}; // Set the camera position
        camera->back = glm::normalize(camera->position - glm::vec3{0.0f, 0.0f, 0.0f});
        camera->update(); // Ensure the view matrix is updated
        scene.camera = std::move(camera);

        // Add room background
        auto background = std::make_unique<RoomBackground>();
        scene.objects.push_back(std::move(background));

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
        auto aquarium = std::make_unique<Aquarium>();
        aquarium->rotation.z = glm::radians(45.0f);
        aquarium->rotation.x = glm::radians(-45.0f);
        aquarium->rotation.y = glm::radians(-90.0f);
        aquarium->position = {-3.5f, 2.0f, -2.0f}; // On the table, closer to camera
        scene.objects.push_back(std::move(aquarium));

        // Initialize camera transition variables
        scene.initialCameraPosition = scene.camera->position;  // Starting position
        // {10.0f, -10.0f, 10.0f}
        scene.targetCameraPosition = {0.5f, 0.0f, -0.0f};      // Near the aquarium
        scene.transitionToNextScene = false;
        scene.transitionProgress = 0.0f;
        scene.nextSceneTriggered = false;
    }

    bool animate = true;

public:
    /*!
     * Construct custom scene window
     */
    SceneWindow() : Window{"gl9_scene", SIZE, SIZE} {
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
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        scene.keyboard[key] = action;

        // Reset
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            initScene();
        }

        // Pause
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            animate = !animate;
        }

        // Start camera transition and switch scene
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !scene.transitionToNextScene) {
            scene.transitionToNextScene = true; // Start the transition
        }
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
        scene.cursor.x = cursorX;
        scene.cursor.y = cursorY;
    }

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse button state
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            scene.cursor.left = action == GLFW_PRESS;

            if (scene.cursor.left) {
                // Convert pixel coordinates to screen coordinates
                double u = (scene.cursor.x / width - 0.5f) * 2.0f;
                double v = -(scene.cursor.y / height - 0.5f) * 2.0f;

                // Get mouse pick vector in world coordinates
                auto direction = scene.camera->cast(u, v);
                auto position = scene.camera->position;

                // Get all objects in scene intersected by ray
                auto picked = scene.intersect(position, direction);

                // Go through all objects that have been picked
                for (auto& obj : picked) {
                    // Pass on the click event
                    obj->onClick(scene);
                }
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            scene.cursor.right = action == GLFW_PRESS;
        }
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        static auto time = (float)glfwGetTime();

        // Compute time delta
        float dt = animate ? (float)glfwGetTime() - time : 0;

        time = (float)glfwGetTime();

        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
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
