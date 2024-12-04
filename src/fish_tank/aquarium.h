//
// Created by Mcham on 11/26/2024.
//
#pragma once

#include <memory>
#include <ppgso/ppgso.h>
#include "scene.h"
#include "object.h"

/*!
 * Aquarium object that displays the loaded glTF model with appropriate textures.
 */
class Aquarium final : public Object {
private:
    // Static resources shared across instances
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create an Aquarium object
     */
    Aquarium();

    /*!
     * Update the aquarium
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return False if the object should be deleted
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render the aquarium
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};