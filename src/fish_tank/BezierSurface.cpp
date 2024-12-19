#include "bezierSurface.h"
#include "scene.h"
#include <glm/gtc/matrix_transform.hpp>

#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
// #include <shaders/bezier_surface_vert_glsl.h>

#include <glm/gtx/normal.hpp>

BezierSurface::BezierSurface() {
    // Initialize the shader
    shader = std::make_unique<ppgso::Shader>(
        ppgso::Shader{"shaders/texture_frag_glsl", "shaders/texture_vert_glsl"}
    );

    // Load the ground texture
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/ground.bmp"));

    // Generate the surface with a resolution of 20x20
    generateSurface(20);

}

BezierSurface::~BezierSurface() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}
glm::vec3 BezierSurface::evaluateBezier(float u, float v) {
    glm::vec3 tempU[4];

    for (int i = 0; i < 4; i++) {
        tempU[i] =
            glm::vec3(pow(1.0f - u, 3.0f)) * controlPoints[i][0] +
            glm::vec3(3.0f * u * pow(1.0f - u, 2.0f)) * controlPoints[i][1] +
            glm::vec3(3.0f * pow(u, 2.0f) * (1.0f - u)) * controlPoints[i][2] +
            glm::vec3(pow(u, 3.0f)) * controlPoints[i][3];
    }

    return glm::vec3(pow(1.0f - v, 3.0f)) * tempU[0] +
           glm::vec3(3.0f * v * pow(1.0f - v, 2.0f)) * tempU[1] +
           glm::vec3(3.0f * pow(v, 2.0f) * (1.0f - v)) * tempU[2] +
           glm::vec3(pow(v, 3.0f)) * tempU[3];
}

glm::vec3 BezierSurface::calculateNormal(float u, float v) {
    float delta = 0.01f;
    glm::vec3 p1 = evaluateBezier(u, v);
    glm::vec3 p2 = evaluateBezier(u + delta, v);
    glm::vec3 p3 = evaluateBezier(u, v + delta);
    return glm::normalize(glm::cross(p2 - p1, p3 - p1));
}

void BezierSurface::generateSurface(int resolution) {
    vertices.clear();
    indices.clear();

    for (int i = 0; i <= resolution; ++i) {
        for (int j = 0; j <= resolution; ++j) {
            float u = static_cast<float>(i) / resolution;
            float v = static_cast<float>(j) / resolution;

            glm::vec3 position = evaluateBezier(u, v);
            glm::vec3 normal = calculateNormal(u, v);
            vertices.push_back({position, normal, {u, v}});
        }
    }

    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            int topLeft = i * (resolution + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (resolution + 1) + j;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

bool BezierSurface::update(Scene& scene, float dt) {
    return true;
}

void BezierSurface::render(Scene& scene) {
    // Use the shader program
    shader->use();

    // Set the uniform matrices
    shader->setUniform("model", glm::mat4(1.0f)); // Identity matrix for the model
    shader->setUniform("view", scene.camera->viewMatrix);
    shader->setUniform("projection", scene.camera->projectionMatrix);
    shader->setUniform("Texture", *texture);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture->id);
    shader->setUniform("textureSampler", 0);

    // Bind the VAO and draw the elements
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

