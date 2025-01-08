// Task 6 - Generate a Bezier surface of variable density with UV coordinates.
//        - Confine the Bezier data and associated methods into a reusable class.
//        - Define a modelMatrix that uses position, rotation, and scale.
//        - Render the generated mesh with texturing applied.
//        - Animate rotation.

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <ppgso/ppgso.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

const unsigned int SIZE = 512;

// Object to represent Bezier patch
class BezierPatch {
private:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> texCoords;
  std::vector<GLuint> indices;

  GLuint vao = 0, vbo = 0, tbo = 0, ibo = 0;
  glm::mat4 modelMatrix{1.0f};

  glm::vec3 bezierPoint(const glm::vec3 controlPoints[4], float t) {
    float it = 1.0f - t;
    return controlPoints[0] * (it * it * it) +
           controlPoints[1] * (3 * it * it * t) +
           controlPoints[2] * (3 * it * t * t) +
           controlPoints[3] * (t * t * t);
  }

  ppgso::Shader program{texture_vert_glsl, texture_frag_glsl};
  ppgso::Texture texture{ppgso::image::loadBMP("lena.bmp")};

public:
  glm::vec3 position{0, 0, 0};
  glm::vec3 rotation{0, 0, 0};
  glm::vec3 scale{1, 1, 1};

  BezierPatch(const glm::vec3 controlPoints[4][4]) {
    unsigned int PATCH_SIZE = 64;
    for (unsigned int i = 0; i < PATCH_SIZE; i++) {
      for (unsigned int j = 0; j < PATCH_SIZE; j++) {
        float u = i / static_cast<float>(PATCH_SIZE - 1);
        float v = j / static_cast<float>(PATCH_SIZE - 1);

        glm::vec3 uCurve[4];
        for (int k = 0; k < 4; k++) {
          uCurve[k] = bezierPoint(controlPoints[k], u);
        }
        glm::vec3 point = bezierPoint(uCurve, v);

        vertices.push_back(point);
        texCoords.push_back({u, v});
      }
    }

    for (unsigned int i = 1; i < PATCH_SIZE; i++) {
      for (unsigned int j = 1; j < PATCH_SIZE; j++) {
        indices.push_back((i - 1) * PATCH_SIZE + (j - 1));
        indices.push_back(i * PATCH_SIZE + (j - 1));
        indices.push_back(i * PATCH_SIZE + j);

        indices.push_back((i - 1) * PATCH_SIZE + (j - 1));
        indices.push_back(i * PATCH_SIZE + j);
        indices.push_back((i - 1) * PATCH_SIZE + j);
      }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    auto position_attrib = program.getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

    auto texCoord_attrib = program.getAttribLocation("TexCoord");
    glEnableVertexAttribArray(texCoord_attrib);
    glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
  }

  ~BezierPatch() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  void update() {
    modelMatrix = glm::translate(glm::mat4{1.0f}, position) *
                  glm::yawPitchRoll(rotation.y, rotation.x, rotation.z) *
                  glm::scale(glm::mat4{1.0f}, scale);
  }

  void render() {
    program.use();
    auto projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 10.0f);
    program.setUniform("ProjectionMatrix", projection);

    auto view = glm::translate(glm::mat4{}, {0.0f, 0.0f, -5.0f});
    program.setUniform("ViewMatrix", view);

    program.setUniform("ModelMatrix", modelMatrix);

    program.setUniform("Texture", texture);

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
  }
};

class BezierSurfaceWindow : public ppgso::Window {
private:
  glm::vec3 controlPoints[4][4]{
      { {-1,1,0}, {-0.5,1,0}, {.5,1,0}, {1,1,3}, },
      { {-1,.5,0}, {-0.5,.5,0}, {.5,.5,0}, {1,.5,0}, },
      { {-1,-.5,0}, {-0.5,-.5,0}, {.5,-.5,0}, {1,-.5,-1}, },
      { {-1,-1,3}, {-0.5,-1,0}, {.5,-1,0}, {1,-1,0}, },
  };

  BezierPatch bezier{controlPoints};

public:
  BezierSurfaceWindow() : Window{"task6_bezier_surface", SIZE, SIZE} {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  }

  void onIdle() final {
    glClearColor(.1f, .1f, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto time = static_cast<float>(glfwGetTime());
    bezier.rotation = {0, time, 0};
    bezier.update();
    bezier.render();
  }
};

int main() {
  auto window = BezierSurfaceWindow{};
  while (window.pollEvents()) {}
  return EXIT_SUCCESS;
}
