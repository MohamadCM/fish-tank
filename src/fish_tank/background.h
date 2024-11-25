#pragma once
#include <memory>
#include <ppgso/ppgso.h>
#include "object.h"

/*!
 * A static fullscreen background object to render a 2D image
 */
class Background final : public Object {
private:
    // Static resources shared between all instances
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create a new Background
     */
    Background();

    /*!
     * Update the background (does nothing as it's static)
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render the background quad with the texture
     */
    void render(Scene &scene) override;
};
