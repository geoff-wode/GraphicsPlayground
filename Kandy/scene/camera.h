#pragma once

#include <cmath>
#include <string>
#include <glm\glm.hpp>

namespace Kandy
{
  namespace Scene
  {
    class Camera
    {
    public:
      Camera();
      glm::dvec3 Position;
      glm::dvec3 Target;
      glm::dvec3 Up;
      double FieldOfViewY;
      double AspectRatio;

      glm::vec3 PositionHigh() const { return glm::vec3(Position); }
      glm::vec3 PositionLow() const { return glm::vec3(Position - glm::dvec3(PositionHigh())); }
      glm::dvec3 Forward() const { return glm::normalize(Target - Position); }
      glm::dvec3 Right() const { return glm::normalize(glm::cross(Forward(), Up)); }
    };

    class PerspectiveCamera : public Camera
    {
    public:
      PerspectiveCamera();
      double NearPlane;
      double FarPlane;
    };

    class OrthographicCamera : public Camera
    {
    public:
      OrthographicCamera();
      double NearPlane;
      double FarPlane;
      double Left;
      double Right;
      double Top;
      double Bottom;
    };
  }
}
