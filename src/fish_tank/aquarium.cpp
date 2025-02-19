//
// Created by Mcham on 11/26/2024.
//

#include "aquarium.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_transparent_frag_glsl.h>

#include "table.h"

// Static resources
std::unique_ptr<ppgso::Mesh> Aquarium::mesh;
std::unique_ptr<ppgso::Shader> Aquarium::shader;
std::unique_ptr<ppgso::Texture> Aquarium::texture;

Aquarium::Aquarium(Object* tableRef) : table(tableRef) {
    // Load shared resources if not already loaded
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_transparent_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("aquarium.gltf");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/glass.bmp"));
    scale = glm::vec3(0.7f, 0.7f, 0.7f);
    table = tableRef;
}

bool Aquarium::update(Scene& scene, float dt)
{
    age += dt;

    if(table)
    {
        position = table->position + offset;
    }

    generateModelMatrix();
    return true;
}

void Aquarium::render(Scene& scene)
{
    // Enable alpha blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending

    shader->use();

    // Set light uniforms
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightPosition", scene.lightSources.back());

    // Set camera matrices
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);

    // Set transparency (this will control the object’s transparency)
    shader->setUniform("Transparency", 0.5f);  // Adjust this as needed for semi-transparency

    // Bind textures (example for base color)
    shader->setUniform("Texture", *texture);
    // Bind additional textures as needed (e.g., normal map, metallic map)

    // Render the mesh
    mesh->render();

    // Disable blending after rendering the transparent object
    glDisable(GL_BLEND);
}
