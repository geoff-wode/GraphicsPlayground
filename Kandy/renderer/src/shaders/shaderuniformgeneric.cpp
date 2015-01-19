#include <memory>
#include <Kandy\renderer\shaders\shaderuniformobserver.h>
#include <Kandy\renderer\shaders\shaderuniformgeneric.h>

using namespace Kandy::Renderer;

//----------------------------------------------------------

// Define the generic Set method.
template <typename T>
void ShaderUniformGeneric<T>::Set(const T& newValue)
{
  if (std::memcmp(&newValue, &value, sizeof(T)))
  {
    value = newValue;
    observer->Modified(this);
  }
}

//----------------------------------------------------------

// Now explicitly instantiate the type-specific variations, neither more nor less.
// Some macro incantations so there need only be a single list of types...
#define KANDY_DECLARE_SHADER_UNIFORM(typeName, type) \
  template void ShaderUniformGeneric<type>::Set(const type&)
#include <Kandy\renderer\shaders\shaderuniformtype.h>
