#include <Kandy\renderer\shaders\shaderuniform.h>

using namespace Kandy::Renderer;

//----------------------------------------------------

ShaderUniform::ShaderUniform(unsigned int location, const char* const name, ShaderUniformObserver* const observer)
  : Location(location),
    Name(name),
    observer(observer)
{
}

//----------------------------------------------------

void ShaderUniform::Apply()
{
}
