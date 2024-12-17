//
// Created by Mcham on 11/26/2024.
//
#pragma once

#include <memory>
#include <ppgso/ppgso.h>
#include "scene.h"
#include "object.h"

/*!
 * Shark object
 */
class Shark final : public Object {
private:
    // Static resources shared across instances
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    glm::vec3 speed;
    glm::vec3 rotMomentum;
    /*!
     * Create an Shark object
     */
    Shark();

    /*!
     * Update the Shark
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return False if the object should be deleted
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render the Shark
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};