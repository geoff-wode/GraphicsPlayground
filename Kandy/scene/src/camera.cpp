#define _USE_MATH_DEFINES
#include <cmath>
#include <scene\camera.h>

using namespace Kandy::Scene;

//--------------------------------------------------

PerspectiveCamera::PerspectiveCamera()
  : NearPlane(0.01),
    FarPlane(64)
{
}

//--------------------------------------------------

OrthographicCamera::OrthographicCamera()
  : NearPlane(0),
    FarPlane(1),
    Left(0),
    Right(1),
    Top(1),
    Bottom(0)
{
}

//--------------------------------------------------

Camera::Camera()
  : Position(0, -1, 0),
    Target(0),
    Up(0, 0, 1),
    FieldOfViewY(M_PI / 6.0),
    AspectRatio(1)
{
}
