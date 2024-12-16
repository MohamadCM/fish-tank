#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>
#include <vector>
#include <glm/glm.hpp>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
public:
 /*!
  * Update all objects in the scene
  * @param time
  */
 void update(float time);

 /*!
  * Render all objects in the scene
  */
 void render();

 /*!
  * Pick objects using a ray
  * @param position - Position in the scene to pick object from
  * @param direction - Direction to pick objects from
  * @return Objects - Vector of pointers to intersected objects
  */
 std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

 /*!
  * Switch to the next scene. Clears current objects and loads new ones.
  */
 void switchToNextScene();

 // Camera object
 std::unique_ptr<Camera> camera;

 // All objects to be rendered in scene
 std::list<std::unique_ptr<Object>> objects;

 // Keyboard state
 std::map<int, int> keyboard;

 // Lights: A list of light positions in the scene
 std::vector<glm::vec3> lightSources;

 // Directional light
 glm::vec3 lightDirection{-1.0f, -1.0f, -1.0f};

 // Ambient light color
 glm::vec3 ambientLight{0.1f, 0.1f, 0.1f};
 float bubbleTimer = 0.0f;

 // Cursor state
 struct {
  double x, y;
  bool left, right;
 } cursor;

 // Transition-related properties
 bool transitionToNextScene = false;   // Flag to start the transition
 bool nextSceneTriggered = false;     // Flag to indicate scene has switched
 float transitionProgress = 0.0f;     // Interpolation progress (0.0 to 1.0)
 int sceneIndex = 0;
 glm::vec3 initialCameraPosition;     // Camera starting position
 glm::vec3 targetCameraPosition;      // Camera target position during transition
};

#endif // _PPGSO_SCENE_H
