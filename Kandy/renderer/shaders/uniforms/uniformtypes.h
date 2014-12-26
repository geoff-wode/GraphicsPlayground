#pragma once

namespace Kandy
{
  namespace Renderer
  {
    struct ShaderUniformType
    {
      enum Enum
      {
        Int,
        Float,
        FloatVector2,
        FloatVector3,
        FloatVector4,
        FloatMatrix22,
        FloatMatrix33,
        FloatMatrix44
      };
    };
  }
}
