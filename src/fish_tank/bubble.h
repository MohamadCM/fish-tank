//
// Created by Mcham on 11/26/2024.
//
#pragma once

#include <memory>
#include <ppgso/ppgso.h>
#include "scene.h"
#include "object.h"

/*!
 * Bubble object
 */
class Bubble final : public Object {
private:
 // Static resources shared across instances
 static std::unique_ptr<ppgso::Mesh> mesh;
 static std::unique_ptr<ppgso::Shader> shader;
 static std::unique_ptr<ppgso::Texture> texture;

public:
 glm::vec3 speed;
 glm::vec3 rotMomentum;
 float age;
 float lifetime;
 float gravityForce = 10.0;
    float gravity = -0.981f;  // Gravity force (downward)
 float buoyantForce = 5.0f;  // Buoyant force (upward) - adjust as needed for desired floating speed

 /*!
  * Create a Bubble object
  */
 Bubble();

 /*!
  * Update the Bubble
  * @param scene Scene to interact with
  * @param dt Time delta for animation purposes
  * @return False if the object should be deleted
  */
 bool update(Scene &scene, float dt) override;

 /*!
  * Render the Bubble
  * @param scene Scene to render in
  */
 void render(Scene &scene) override;
};