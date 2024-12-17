#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Simple table object
 * It initializes and loads all resources only once
 */
class Table final : public Object
{
private:
 // Static resources (Shared between instances)
 static std::unique_ptr<ppgso::Mesh> mesh;
 static std::unique_ptr<ppgso::Shader> shader;
 static std::unique_ptr<ppgso::Texture> texture;

 // Age of the object in seconds
 float age{0.0f};

public:
 /*!
  * Create new table
  */
 Table();

 /*!
  * Update table
  * @param scene Scene to interact with
  * @param dt Time delta for animation purposes
  * @return
  */
 bool update(Scene& scene, float dt) override;

 /*!
  * Render table
  * @param scene Scene to render in
  */
 void render(Scene& scene) override;

private:
};

