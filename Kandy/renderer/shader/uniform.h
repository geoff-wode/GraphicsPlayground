#pragma once

#include <string>
#include <boost\noncopyable.hpp>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\shader\uniformobserver.h>

namespace Kandy
{
  namespace Renderer
  {
    // Note the somewhat down-side up class naming here:
    // Although "Uniform" is _actually_ at the root of the class hierarchy, it
    // really is the object type being manipulated, while "UniformBase" is the
    // class from which instances of type "UniformXXX" are derived, as in
    // "UniformInt", "UniformFloatVector4", etc.
    // The syntax of the most-derived concrete classes reflects this "base class"
    // relationship, as in:
    //    class UniformFloatVector4 : public UniformBase<glm::vec4>

    class Uniform : public boost::noncopyable
    {
    public:
      struct Type
      {
        enum Enum
        {
          None,
          Int,
          Float,
          FloatVector2,
          FloatVector3,
          FloatVector4,
          FloatMatrix4x4
        };
      };
      
      typedef boost::shared_ptr<Uniform> Ptr;

      const std::string name;
      const Type::Enum  type;
      const unsigned int location;

      virtual ~Uniform() { }

      // Cause the GPU value to be updated from the CPU side
      void Update();

    protected:
      Uniform(IUniformObserver* observer, const std::string& name, Type::Enum type, unsigned int location, void* data)
        : name(name),
          type(type),
          location(location),
          changed(false),
          data(data),
          observer(observer)
      {
      }

      bool changed;
      void* data;
      IUniformObserver* observer;
    };

    template <typename T>
    class UniformBase : public Uniform
    {
    public:
      T Get() const { return value; }

      void Set(const T& value)
      {
        if (!changed && (value != this->value))
        {
          changed = true;
          observer->HasChanged(this);
        }
        this->value = value;
      }

    protected:
      UniformBase(IUniformObserver* observer, const std::string& name, Uniform::Type::Enum type, unsigned int location)
        : Uniform(observer, name, type, location, &value)
      {
      }

      T value;
    };
  }
}
