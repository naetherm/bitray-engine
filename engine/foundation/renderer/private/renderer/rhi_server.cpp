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
#include "renderer/renderer/rhi_server.h"
#include <core/log/log.h>
#include <core/platform/platform.h>
#include <rhi/rhidynamicrhi.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace renderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
RhiServer::RhiServer()
: mRhiDynLib(nullptr)
, mRHIContext(nullptr)
, mRHIDevice(nullptr) {

}

RhiServer::~RhiServer() {

}


void RhiServer::initialize(const core::String& rhiName, rhi::RHIContext* rhiContext) {
  mRHIName = rhiName;
  mRHIContext = rhiContext;

  create_rhi_device();
}

void RhiServer::initialize(rhi::RHIDevice* rhiDevice) {
  // Nothing else to do
  mRHIDevice = rhiDevice;
}


void RhiServer::create_rhi_device() {
  core::String libraryName = core::Platform::instance().get_shared_library_prefix() + "rhi_" + mRHIName + "." + core::Platform::instance().get_shared_library_extension();

  mRhiDynLib = new core::Library();
  mRhiDynLib->set_path(core::Path(libraryName));

  if (mRhiDynLib->load()) {
    typedef rhi::RHIDevice* (*RHI_CREATE_FUNC)();
    RHI_CREATE_FUNC _CreateRHIDevice = reinterpret_cast<RHI_CREATE_FUNC>(mRhiDynLib->get_symbol("create_rhi_device_instance"));
    if (_CreateRHIDevice) {
      mRHIDevice = _CreateRHIDevice();
      mRHIDevice->initialize(*mRHIContext);

      if (mRHIDevice) {
        mRHIDevice->add_ref();
        BE_LOG(Info, core::String("Successfully created the RHI backend of ") + libraryName)
      } else {
        // Error
        BE_LOG(Error, "Creation of dynamic RHI failed.")
      }
    } else {
      // Error
      BE_LOG(Error, "Unable to find the symbol 'create_rhi_instance'")
    }
  } else {
    // Error
    BE_LOG(Error, core::String("Unable to load the library ") + libraryName)
  }
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}