// Task 5 - Draw a 2D shape using polygons and animate it
//        - Encapsulate the shape using a class
//        - Use color_vert/frag shader to display the polygon
//        - Animate the object position, rotation and scale.

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

// Object to represent 2D OpenGL shape
class Shape {
private:
  // Define your shape points (a triangle in this case)
  std::vector<glm::vec3> vertices {
      {0.0f, 0.5f, 0.0f},   // Top vertex
      {-0.5f, -0.5f, 0.0f}, // Bottom left vertex
      {0.5f, -0.5f, 0.0f}   // Bottom right vertex
  };

  // Define your face structure
  struct Face {
    unsigned int indices[3];
  };

  // Define mesh indices for the triangle
  std::vector<Face> mesh {
      {{0, 1, 2}} // Single triangle
  };

  // Program to associate with the object
  ppgso::Shader program = {color_vert_glsl, color_frag_glsl};

  // Data and object buffers
  GLuint vao, vbo, ibo;
  glm::mat4 modelMatrix{1.0f};
public:
  // Public attributes
  glm::vec3 position{0.5,0.5,0};
  glm::vec3 rotation{-0.5,-0.5,0};
  glm::vec3 scale{1,1,1};
  glm::vec3 color{1,0,0};

  // Initialize object data buffers
  Shape() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Copy positions to GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = program.getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy mesh indices to GPU
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(Face), mesh.data(), GL_STATIC_DRAW);

    program.setUniform("ProjectionMatrix", glm::mat4{1.0f});
    program.setUniform("ViewMatrix", glm::mat4{1.0f});

    modelMatrix = glm::mat4(1.0f);
  }

  // Clean up
  ~Shape() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }
  // Set the object transformation matrix
  void update() {
    modelMatrix = glm::translate(glm::mat4(1.0f), position)
                      * glm::eulerAngleXY(rotation.x, rotation.y)
                      * glm::scale(glm::mat4(1.0f), scale);
  }

  // Draw polygons
  void render() {
    program.use();
    program.setUniform("OverallColor", color);
    program.setUniform("ModelMatrix", modelMatrix);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh.size() * 3, GL_UNSIGNED_INT, 0);
  }
};

class ShapeWindow : public ppgso::Window {
private:
  Shape shape1, shape2;
public:
  ShapeWindow() : Window{"task4_2dshapes", SIZE, SIZE} {
    shape1.color = {1, 0, 1}; // Red
    shape2.color = {1, 1, 0}; // Yellow
  }

  void onIdle() {
    // Set gray background
    glClearColor(.1f,.1f,.1f,1.0f);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move and Render shape\    // Get time for animation
    auto t = (float) glfwGetTime();

    // TODO: manipuate shape1 and shape2 position to rotate clockwise
    shape1.position = { cos(-t), sin(-t), 0 };
    shape2.position = -shape1.position;

    // Manipulate rotation of the shape
    shape1.rotation.z = t * 5.0f;
    shape2.rotation = -shape1.rotation;

    // Manipulate shape size
    shape1.scale = {sin(t),sin(t), 1};
    shape2.scale = -shape1.scale;

    // Update and render each shape
    shape1.update();
    shape2.update();

    shape1.render();
    shape2.render();
  }
};

int main() {
  // Create our window
  auto window = ShapeWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
