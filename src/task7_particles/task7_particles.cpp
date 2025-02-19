// Task 7 - Dynamically generate objects in a 3D scene
//        - Implement a particle system where particles have position and speed
//        - Any object can be a generator and can add objects to the scene
//        - Create dynamic effect such as fireworks, rain etc.
//        - Encapsulate camera in a class

#include <iostream>
#include <vector>
#include <map>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

class Camera
{
public:
    // TODO: Add parameters
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    /// Representaiton of
    /// \param fov - Field of view (in degrees)
    /// \param ratio - Viewport ratio (width/height)
    /// \param near - Distance of the near clipping plane
    /// \param far - Distance of the far clipping plane
    Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f)
    {
        // TODO: Initialize perspective projection (hint: glm::perspective)
        projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
    }

    /// Recalculate viewMatrix from position, rotation and scale
    void update()
    {
        // TODO: Update viewMatrix (hint: glm::lookAt)
        glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f); // Camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // Target the origin
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Up direction
        viewMatrix = glm::lookAt(cameraPosition, cameraTarget, up);
    }
};

/// Abstract renderable object interface
class Renderable; // Forward declaration for Scene
using Scene = std::list<std::unique_ptr<Renderable>>; // Type alias

class Renderable
{
public:
    // Virtual destructor is needed for abstract interfaces
    virtual ~Renderable() = default;

    /// Render the object
    /// \param camera - Camera to use for rendering
    virtual void render(const Camera& camera) = 0;

    /// Update the object. Useful for specifing animation and behaviour.
    /// \param dTime - Time delta
    /// \param scene - Scene reference
    /// \return - Return true to keep object in scene
    virtual bool update(float dTime, Scene& scene) = 0;
};

/// Basic particle that will render a sphere
/// TODO: Implement Renderable particle
class Particle final : public Renderable
{
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    // TODO: add more parameters as needed
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 color;

public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c) : position(p), speed(s), color(c)
    {
        // First particle will initialize resources
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    }

    bool update(float dTime, Scene& scene) override
    {
        // TODO: Animate position using speed and dTime.
        // - Return true to keep the object alive
        // - Returning false removes the object from the scene
        // - hint: you can add more particles to the scene here also
        position += speed * dTime;

        // Example condition to remove particle after a certain distance
        if (position.y < -10.0f)
            return false;

        // TODO: Add new particles (for effects like fireworks, rain)
        if (position.y > 5.0f)
        {
            // Add new particles to the scene for fireworks
            scene.push_back(std::move(std::make_unique<Particle>(glm::vec3(position.x, position.y, position.z),
                                                       glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f))));
        }

        return true;
    }

    void render(const Camera& camera) override
    {
        // TODO: Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh
        shader->use();
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};

// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> Particle::mesh;
std::unique_ptr<ppgso::Shader> Particle::shader;

class ParticleWindow : public ppgso::Window
{
private:
    // Scene of objects
    Scene scene;

    // Create camera
    Camera camera = {120.0f, (float)width / (float)height, 1.0f, 400.0f};

    // Store keyboard state
    std::map<int, int> keys;

public:
    ParticleWindow() : Window{"task7_particles", SIZE, SIZE}
    {
        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    void onKey(int key, int scanCode, int action, int mods) override
    {
        // Collect key state in a map
        keys[key] = action;
        if (keys[GLFW_KEY_SPACE])
        {
            double randomNumber = (static_cast<double>(std::rand()) / RAND_MAX) * 2.0 - 1.0;
            // TODO: Add renderable object to the scene
            scene.push_back(std::move(std::make_unique<Particle>(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(randomNumber, -1.0f, 0.0f),
                                                       glm::vec3(0.3f, 0.3f, 0.0f))));
        }
    }

    void onIdle() override
    {
        // Track time
        static auto time = (float)glfwGetTime();
        // Compute time delta
        float dTime = (float)glfwGetTime() - time;
        time = (float)glfwGetTime();

        // Set gray background
        glClearColor(.1f, .1f, .1f, 1.0f);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update all objects in scene
        // Because we need to delete while iterating this is implemented using c++ iterators
        // In most languages mutating the container during iteration is undefined behaviour
        auto i = std::begin(scene);
        while (i != std::end(scene))
        {
            // Update object and remove from list if needed
            auto obj = i->get();
            if (!obj->update(dTime, scene))
                i = scene.erase(i);
            else
                ++i;
        }

        camera.update();
        // Render every object in scene
        for (auto& object : scene)
        {
            object->render(camera);
        }
    }

    // Access the scene object
    Scene& getScene()
    {
        return scene;
    }
};

int main()
{
    // Create new window
    auto window = ParticleWindow{};

    std::srand(std::time(0));
    // Main execution loop
    while (window.pollEvents())
    {
    }

    return EXIT_SUCCESS;
}
