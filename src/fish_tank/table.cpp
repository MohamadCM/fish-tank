#include <glm/gtc/random.hpp>
#include "table.h"
// #include "projectile.h"
// #include "explosion.h"

#include <shaders/advanced_material_vert_glsl.h>
#include <shaders/advanced_material_frag_glsl.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Table::mesh;
std::unique_ptr<ppgso::Texture> Table::texture;
std::unique_ptr<ppgso::Shader> Table::shader;

Table::Table()
{
    // Set  scale, speed, and rotation
    scale = glm::vec3(5.0f, 5.0f, 5.0f); // This is the default scale, which makes the object 1x in size.

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/wood.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("table.obj");
}

bool Table::update(Scene& scene, float dt)
{
    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();
    return true;
}


void Table::render(Scene& scene)
{
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("LightPosition", scene.lightSources.back());
    // shader->setUniform("LightColor", glm::vec3(1.0f, 0.5f, 0.5f));

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("Texture", *texture);
    mesh->render();
}
