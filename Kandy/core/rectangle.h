#pragma once

namespace Kandy
{
  namespace Core
  {
    struct Rectangle
    {
      Rectangle();
      Rectangle(const Rectangle& other);
      Rectangle(int x, int y, size_t width, size_t height);

      int x;
      int y;
      size_t width;
      size_t height;
    };

    bool operator==(const Rectangle& a, const Rectangle& b);
    bool operator!=(const Rectangle& a, const Rectangle& b);
  }
}
