// Task 4 - Render the letter R using OpenGL
//        - Implement a function to generate Bézier curve points
//        - Generate multiple points on each Bézier curve
//        - Draw lines connecting the generated points using OpenGL

#include <iostream>
#include <vector>
#include <cmath>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

class BezierWindow : public ppgso::Window {
private:
  // Control points for the Bézier curves defining the letter "R"
  std::vector<glm::vec2> controlPoints = {
      { 0,  -1},
      { 0, -.3},
      { 0,  .3},
      { 0,   1},
      {.3,   1},
      {.5,   1},
      {.5,  .5},
      {.5,   0},
      {.3,   0},
      { 0,   0},
      {.3, -.3},
      {.5, -.5},
      {.5,  -1},
  };

  // This will hold the Bézier curve geometry once we generate it
  std::vector<glm::vec3> points;

  // GLSL Program to use for rendering
  ppgso::Shader program = {color_vert_glsl, color_frag_glsl};

  // Vertex Array Object and Vertex Buffer Object identifiers
  GLuint vao = 0, vbo = 0;

  // Compute point on a cubic Bézier curve using 4 control points
  glm::vec2 bezierPoint(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3, const float t) {
    // De Casteljau's algorithm for a cubic Bezier curve
    return (1 - t) * (1 - t) * (1 - t) * p0 +
           3 * (1 - t) * (1 - t) * t * p1 +
           3 * (1 - t) * t * t * p2 +
           t * t * t * p3;
  }

  void bezierShape(int count) {
    for (int i = 0; i + 3 < controlPoints.size(); i += 3) {
      for (int j = 0; j <= count; j++) {
        float t = j / static_cast<float>(count);
        glm::vec2 point = bezierPoint(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], t);
        points.emplace_back(point, 0);
      }
    }
  }

public:
  BezierWindow() : Window{"task4_bezier", SIZE, SIZE} {
    // Generate Bezier curve points
    bezierShape(15);

    // Generate a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Pass the generated points to the GPU
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

    // Setup vertex array attribute for position
    auto position_attrib = program.getAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(position_attrib);

    // Set the identity matrices for the shader program
    program.setUniform("ModelMatrix", glm::mat4{});
    program.setUniform("ViewMatrix", glm::mat4{});
    program.setUniform("ProjectionMatrix", glm::mat4{});

    // Set the color of the letter "R" in white
    program.setUniform("OverallColor", glm::vec3{1.0f, 1.0f, 1.0f});
  }

  ~BezierWindow() final {
    // Clean up allocated resources
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  void onIdle() final {
    // Set gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program
    program.use();

    // Bind the vertex array object and render the points as a line strip
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_STRIP, 0, points.size());
  }
};

int main() {
  // Create the window for rendering
  BezierWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
