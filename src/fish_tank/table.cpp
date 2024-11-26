#include <glm/gtc/random.hpp>
#include "table.h"
// #include "projectile.h"
// #include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Table::mesh;
std::unique_ptr<ppgso::Texture> Table::texture;
std::unique_ptr<ppgso::Shader> Table::shader;

Table::Table() {
  // Set  scale, speed, and rotation
  scale = glm::vec3(5.0f, 5.0f, 5.0f); // This is the default scale, which makes the object 1x in size.
  speed = {glm::linearRand(-2.0f, 2.0f), glm::linearRand(-5.0f, -10.0f), 0.0f};
  // rotation = glm::ballRand(ppgso::PI);
  // rotMomentum = glm::ballRand(ppgso::PI);

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("wood.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("table.obj");
}

bool Table::update(Scene &scene, float dt) {
  // Count time alive
  // age += dt;

  // Animate position according to time
  // position += speed * dt;

  // Rotate the object
  rotation += rotMomentum * dt;

  // Delete when alive longer than 10s or out of visibility
  if (age > 10.0f || position.y < -10) return false;

  // Collide with scene
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with tables and projectiles, ignore other objects
    auto table = dynamic_cast<Table*>(obj.get()); // dynamic_pointer_cast<table>(obj);
    // auto projectile = dynamic_cast<Projectile*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
    // if (!table && !projectile) continue;

    // When colliding with other tables make sure the object is older than .5s
    // This prevents excessive collisions when tables explode.
    if (table && age < 0.5f) continue;

    // Compare distance to approximate size of the table estimated from scale.
    if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
      int pieces = 3;

      // Too small to split into pieces
      if (scale.y < 0.5) pieces = 0;

      // The projectile will be destroyed
      // if (projectile) projectile->destroy();

      // Generate smaller tables
      // explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

      // Destroy self
      return false;
    }
  }

  // Generate modelMatrix from position, rotation and scale
  generateModelMatrix();

  return true;
}


void Table::render(Scene &scene) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  shader->setUniform("LightPosition", scene.lightSources.back());
  shader->setUniform("LightColor", glm::vec3(1.0f, 1.0f, 0.9f)); // Warm light color

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Table::onClick(Scene &scene) {
  // std::cout << "table clicked!" << std::endl;
  // explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
  // age = 10000;
}
