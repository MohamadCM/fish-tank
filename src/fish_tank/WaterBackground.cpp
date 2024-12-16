#include "WaterBackground.h"
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> WaterBackground::mesh;
std::unique_ptr<ppgso::Shader> WaterBackground::shader;
std::unique_ptr<ppgso::Texture> WaterBackground::texture;

WaterBackground::WaterBackground() {
    // Initialize static resources
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water_background.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj"); // A flat square covering [-1, 1] range
}

bool WaterBackground::update(Scene &scene, float dt) {
    // Background is static, no update required
    return true;
}

void WaterBackground::render(Scene &scene) {
    // Disable depth writing to ensure the background stays behind all objects
    glDepthMask(GL_FALSE);

    shader->use();

    // Use orthographic projection to render the background as a flat quad
    glm::mat4 orthoProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    shader->setUniform("ProjectionMatrix", orthoProjection);
    shader->setUniform("ModelMatrix", glm::mat4(1.0f)); // No transformations needed

    // Bind the texture
    shader->setUniform("BaseColorTexture", *texture);

    // Render the quad
    mesh->render();

    // Re-enable depth writing
    glDepthMask(GL_TRUE);
}
