#pragma once

#include <vector>
#include <Kandy\renderer\buffers\vertexbuffer.h>
#include <Kandy\renderer\buffers\indexbuffer.h>
#include <Kandy\renderer\vertexarrayattribute.h>

namespace Kandy
{
  namespace Renderer
  {
    struct VertexArray
    {
      VertexArray();
      ~VertexArray();

      size_t Stride() const;

      IndexBuffer* Indices;
      VertexBuffer* Vertices;

      const std::vector<VertexArrayAttribute>& GetAttributes() const;

      bool AddAttribute(const VertexArrayAttribute& attr);

      void Bind() const;
      static void Unbind();

    private:
      struct Impl;
      Impl* impl;
    };
  }
}
