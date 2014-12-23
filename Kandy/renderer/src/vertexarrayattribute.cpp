#include <cstdint>
#include <core\logging.h>
#include <renderer\vertexarrayattribute.h>
#include "gl_core_3_3.hpp"
#include "gltypeconversion.h"

using namespace Kandy::Renderer;

//----------------------------------------------------------

static size_t DataTypeSize(ComponentType::Enum type)
{
  switch (type)
  {
  case ComponentType::Byte:
  case ComponentType::UnsignedByte:
    return sizeof(uint8_t);
    break;
  case ComponentType::Short:        
  case ComponentType::UnsignedShort:
    return sizeof(uint16_t);
    break;
  case ComponentType::Int:          
  case ComponentType::UnsignedInt:  
    return sizeof(uint32_t);
    break;
  case ComponentType::Float:        
    return sizeof(float);
    break;
  case ComponentType::HalfFloat:    
    return sizeof(float) / 2;
    break;
  default:
    ASSERT(false);
    break;
  }
  return 0;
}

//----------------------------------------------------------

size_t VertexArrayAttribute::SizeInBytes() const
{
  return componentCount * DataTypeSize(type);
}
