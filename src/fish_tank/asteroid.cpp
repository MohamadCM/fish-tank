#include <glm/gtc/random.hpp>
#include "asteroid.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Asteroid::mesh;
std::unique_ptr<ppgso::Texture> Asteroid::texture;
std::unique_ptr<ppgso::Shader> Asteroid::shader;

Asteroid::Asteroid() {
  // Set random scale speed and rotation
  scale *= glm::linearRand(1.0f, 3.0f);
  speed = {glm::linearRand(-2.0f, 2.0f), glm::linearRand(-5.0f, -10.0f), 0.0f};
  rotation = glm::ballRand(ppgso::PI);
  rotMomentum = glm::ballRand(ppgso::PI);

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/asteroid.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("asteroid.obj");
}

bool Asteroid::update(Scene &scene, float dt) {
  // Generate modelMatrix from position, rotation and scale
  generateModelMatrix();

  return true;
}

void Asteroid::explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces) {
  // Generate explosion
  auto explosion = std::make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed = speed / 2.0f;
  scene.objects.push_back(move(explosion));

  // Generate smaller asteroids
  for (int i = 0; i < pieces; i++) {
    auto asteroid = std::make_unique<Asteroid>();
    asteroid->speed = speed + glm::vec3(glm::linearRand(-3.0f, 3.0f), glm::linearRand(0.0f, -5.0f), 0.0f);;
    asteroid->position = position;
    asteroid->rotMomentum = rotMomentum;
    float factor = (float) pieces / 2.0f;
    asteroid->scale = scale / factor;
    scene.objects.push_back(move(asteroid));
  }
}

void Asteroid::render(Scene &scene) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Asteroid::onClick(Scene &scene) {
  std::cout << "Asteroid clicked!" << std::endl;
  explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
  age = 10000;
}

