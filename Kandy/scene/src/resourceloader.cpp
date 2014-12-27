#include <windows.h>

#include <Kandy\scene\resourceloader.h>
#include <Kandy\core\logging.h>

using namespace Kandy::Core;

void ResourceLoader::Load(ResourceLoader::Resource& res)
{
  HMODULE handle = ::GetModuleHandle(NULL);
  HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(res.id), MAKEINTRESOURCE(res.type));
  HGLOBAL rcData = ::LoadResource(handle, rc);
  res.size = ::SizeofResource(handle, rc);
  res.data = static_cast<const char*>(::LockResource(rcData));
}
