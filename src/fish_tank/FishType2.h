#include <memory>
#include <ppgso/ppgso.h>
#include "scene.h"
#include "object.h"

/*!
 * FishType2 object
 */
class FishType2 final : public Object {
private:
    // Static resources shared across instances
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    glm::vec3 speed;
    glm::vec3 rotMomentum;
    /*!
     * Create an FishType2 object
     */
    FishType2();

    /*!
     * Update the FishType2
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return False if the object should be deleted
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render the FishType2
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};