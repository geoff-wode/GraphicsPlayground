#include <Kandy\core\rectangle.h>

//-----------------------------------------------

Kandy::Core::Rectangle::Rectangle()
  : x(0), y(0), width(0), height(0)
{
}

//-----------------------------------------------

Kandy::Core::Rectangle::Rectangle(const Rectangle& other)
  : x(other.x), y(other.y), width(other.width), height(other.height)
{
}

//-----------------------------------------------

Kandy::Core::Rectangle::Rectangle(int x, int y, size_t width, size_t height)
  : x(x), y(y), width(width), height(height)
{
}

//-----------------------------------------------

bool Kandy::Core::operator==(const Rectangle& a, const Rectangle& b)
{
  return ((a.x == b.x) && (a.y == b.y) && (a.width == b.width) && (a.height == b.height));
}

//-----------------------------------------------

bool Kandy::Core::operator!=(const Rectangle& a, const Rectangle& b)
{
  return !(a == b);
}
