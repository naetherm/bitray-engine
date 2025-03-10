////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 RacoonStudios
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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
#include "gui/gui/gui_context.h"
#include <core/log/log.h>
#include <core/platform/platform.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiContext::GuiContext()
: mRhiLibrary(nullptr) {

}

GuiContext::~GuiContext() {
  if (mRhiLibrary) {
    re_delete(mRhiLibrary);
  }
}


bool GuiContext::initialize_by_name(const core::String &rhiName) {
  mGuiSettings.mRhiName = rhiName;
  mRhiLibraryName = core::Platform::instance().get_shared_library_prefix() + "rhi_" + mGuiSettings.mRhiName + "." +
                    core::Platform::instance().get_shared_library_extension();

  mRhiLibrary = re_new<core::Library>();
  mRhiLibrary->set_path(core::Path(mRhiLibraryName));
  // Load shared dynamic library
  if (mRhiLibrary->load()) {
    BE_LOG(Info, "Successfully loaded library " + mRhiLibraryName)


    return true;
  }

  BE_LOG(Critical, "Unable to load library " + mRhiLibraryName)

  // Clean library
  if (mRhiLibrary) {
    delete mRhiLibrary;
    mRhiLibrary = nullptr;
  }

  return false;
}

bool GuiContext::initialize(const GuiSettings &guiSettings) {
  return initialize_by_name(guiSettings.mRhiName);
}

const core::String& GuiContext::get_rhi_name() const {
  return mRhiName;
}

const core::String & GuiContext::get_rhi_library_name() const {
  return mRhiLibraryName;
}

core::Library& GuiContext::get_rhi_library() const {
  return *mRhiLibrary;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}