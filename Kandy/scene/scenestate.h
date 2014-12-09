#pragma once

#include <glm\glm.hpp>

namespace Kandy
{
  namespace Scene
  {
    struct SceneState
    {
      // Keep this list in sync with renderer\shader\autouniforms\list.h...
      glm::vec3 cameraPosition;
      glm::mat4 modelMatrix;
      glm::mat4 viewMatrix;
      glm::mat4 projectionMatrix;
      glm::mat4 viewProjectionMatrix;
      glm::mat4 modelViewProjectionMatrix;
    };
  }
}
