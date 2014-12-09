#include <Kandy\renderer\shader\uniform.h>
#include <renderer\src\gl_core_3_3.hpp>

using namespace Kandy;
using namespace Kandy::Renderer;

void Uniform::Update()
{
  if (changed)
  {
    switch (type)
    {
    case Type::Int:           gl::Uniform1iv(location, 1, (const GLint*)data); break;
    case Type::Float:         gl::Uniform1fv(location, 1, (const GLfloat*)data); break;
    case Type::FloatVector2:  gl::Uniform2fv(location, 1, (const GLfloat*)data); break;
    case Type::FloatVector3:  gl::Uniform3fv(location, 1, (const GLfloat*)data); break;
    case Type::FloatVector4:  gl::Uniform4fv(location, 1, (const GLfloat*)data); break;
    case Type::FloatMatrix4x4:gl::UniformMatrix4fv(location, 1, gl::FALSE_, (const GLfloat*)data); break;
    default: break;
    }
    changed = false;
  }
}


