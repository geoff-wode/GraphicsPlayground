#pragma once

#include <boost\shared_ptr.hpp>

namespace Kandy
{
  namespace Core
  {
    namespace ResourceLoader
    {
      struct Resource
      {
        int id;
        int type;
        unsigned long size;
        const char* data;
      };

      void Load(Resource& res);
    }
  }
}
