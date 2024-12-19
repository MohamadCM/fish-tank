//
// Created by Mohamad Chamanmotlagh on 11/26/2024.
//
#pragma once
#include <memory>
#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Lamp object that uses a glTF model.
 * Supports base color, metallic-roughness, and normal textures.
 */
class Lamp final : public Object {
private:
 float elapsedTime = 0.0f; // Accumulator for movement
 static std::unique_ptr<ppgso::Mesh> mesh;
 static std::unique_ptr<ppgso::Shader> shader;
 static std::unique_ptr<ppgso::Texture> baseColor;
 static std::unique_ptr<ppgso::Texture> metallicRoughness;
 static std::unique_ptr<ppgso::Texture> normalMap;

public:
 // Position of the light coming out of the lamp
 glm::vec3 lightPosition;
 /*!
  * Create a new Lamp instance
  */
 Lamp();

 /*!
  * Update the lamp
  * @param scene Scene to interact with
  * @param dt Time delta for animations
  * @return true if the object should be kept in the scene
  */
 bool update(Scene &scene, float dt) override;

 /*!
  * Render the lamp
  * @param scene Scene to render in
  */
 void render(Scene &scene) override;
};
