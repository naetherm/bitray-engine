////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/linux/linux_library.h"
#if !defined(ANDROID) && !defined(APPLE)
#include <link.h>	// For "dlinfo", not available on Android and Mac OS X
#endif
#include <dlfcn.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
LinuxLibrary::LinuxLibrary()
: mLib(nullptr) {

}

LinuxLibrary::~LinuxLibrary() {
  this->unload();
}

bool LinuxLibrary::is_loaded() const {
  return (this->mLib != nullptr);
}

bool LinuxLibrary::load() {
  if (this->mLib) {
    this->unload();
  }

  this->mLib = dlopen(mLibraryPath.get_native_path().c_str(), RTLD_NOW | RTLD_GLOBAL);

  if (mLib) {
    // Done
    return true;
  }

  // Error: Failed to load dll!
  printf("Unable to load library due to %s\n", dlerror());

  return false;
}

String LinuxLibrary::get_abs_path() const {
#if !defined(ANDROID) && !defined(APPLE)
  // "dlinfo" is not available on Android and Mac OS X
  link_map *pLinkMap = nullptr;
  if (mLib && !dlinfo(mLib, RTLD_DI_LINKMAP, &pLinkMap)) {
    // Success
    return String(pLinkMap->l_name);
  }
#endif

  // Error!
  return "";
}

bool LinuxLibrary::unload() {
  if (mLib) {
    // Unload library
    if (!dlclose(mLib)) {
      mLib = nullptr;

      // Done
      return true;
    }
  }

  // Error, could not unload the library
  return false;
}

void *LinuxLibrary::get_symbol(const String &sSymbol) const {
  return mLib ? dlsym(mLib, sSymbol.c_str()) : nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core