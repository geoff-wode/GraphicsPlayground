#pragma once

namespace Kandy
{
  namespace Renderer
  {
    struct DepthTest
    {
      struct Function
      {
        enum Enum
        {
          Never,
          Less,
          Equal,
          LessThanOrEqual,
          Greater,
          NotEqual,
          GreaterThanOrEqual,
          Always
        };
      };
      bool enabled;
      Function::Enum function;
      DepthTest() : enabled(true), function(Function::Less) { }
    };
  }
}
