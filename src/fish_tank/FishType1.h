//
// Created by Mcham on 11/26/2024.
//
#pragma once

#include <memory>
#include <ppgso/ppgso.h>
#include "scene.h"
#include "object.h"

/*!
 * FishType1 object
 */
class FishType1 final : public Object
{
private:
 // Static resources shared across instances
 static std::unique_ptr<ppgso::Mesh> mesh;
 static std::unique_ptr<ppgso::Shader> shader;
 static std::unique_ptr<ppgso::Texture> texture;

public:
 glm::vec3 speed;
 glm::vec3 rotMomentum;
 /*!
  * Create an FishType1 object
  */
 FishType1();

 /*!
  * Update the FishType1
  * @param scene Scene to interact with
  * @param dt Time delta for animation purposes
  * @return False if the object should be deleted
  */
 bool update(Scene& scene, float dt) override;

 /*!
  * Render the FishType1
  * @param scene Scene to render in
  */
 void render(Scene& scene) override;
 void fleeFrom(const glm::vec3& predatorPosition, float fleeSpeed, float dt);
};
