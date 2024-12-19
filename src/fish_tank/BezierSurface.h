#ifndef BEZIER_SURFACE_H
#define BEZIER_SURFACE_H

#include <ppgso/ppgso.h>
#include "object.h"

class BezierSurface : public Object {
private:
    struct Vertex {
        glm::vec3 position;  // Vertex position
        glm::vec3 normal;    // Vertex normal
        glm::vec2 texCoord;  // Texture coordinates
    };

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint vao, vbo, ebo;   // OpenGL buffers
    std::unique_ptr<ppgso::Texture> texture;         // Texture for the ground

    std::unique_ptr<ppgso::Shader> shader;

    // Control points for the Bezier surface (4x4 grid)
    glm::vec3 controlPoints[4][4] = {
        {{-10, 0, -10}, {-5, 1, -10}, {5, -1, -10}, {10, 0, -10}},
        {{-10, 0, -5},  {-5, 2, -5},  {5, -2, -5},  {10, 0, -5}},
        {{-10, 0, 5},   {-5, -1, 5},  {5, 1, 5},   {10, 0, 5}},
        {{-10, 0, 10},  {-5, -1, 10}, {5, 1, 10},  {10, 0, 10}}
    };

    glm::vec3 evaluateBezier(float u, float v);
    glm::vec3 calculateNormal(float u, float v);
    void generateSurface(int resolution);

public:
    BezierSurface();
    ~BezierSurface();

    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;
};

#endif // BEZIER_SURFACE_H
