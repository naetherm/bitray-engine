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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "rhi/rhi.h"
#include <core/frontend/types.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Context class encapsulating all embedding related wirings
 */
class RHIContext {

  // Public methods
public:
  /**
   * @brief
   * Constructor
   *
   * @param[in] log
   * Log instance to use, the log instance must stay valid as long as the RHI instance exists
   * @param[in] assert
   * Assert instance to use, the assert instance must stay valid as long as the RHI instance exists
   * @param[in] allocator
   * Allocator instance to use, the allocator instance must stay valid as long as the RHI instance exists
   * @param[in] nativeWindowHandle
   * Native window handle
   * @param[in] useExternalContext
   * Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
   * @param[in] contextType
   * The type of the context
   */
  inline RHIContext(core::FrontendPlatformData frontendPlatformData, bool useExternalContext = false)
  : mNativeWindowHandle(NULL_HANDLE)
  , mUseExternalContext(useExternalContext)
  , mRhiApiSharedLibrary(nullptr)
  , mPlatformData(frontendPlatformData) {

  }

  /**
   * @brief
   * Destructor
   */
  inline virtual ~RHIContext() {

  }

  /**
   * @brief
   * Return the native window handle
   *
   * @return
   * The native window handle
   */
  [[nodiscard]] inline core::handle get_native_window_handle() const {
    return mNativeWindowHandle;
  }

  /**
   * @brief
   * Return the frontend platform data information.
   *
   * @return
   * The frontend platform data information.
   */
  [[nodiscard]] inline const core::FrontendPlatformData& get_frontend_platform_data() const {
    return mPlatformData;
  }

  /**
   * @brief
   * Return whether or not an external context is used
   *
   * @return
   * "true" if an external context is used, else "false"
   */
  [[nodiscard]] inline bool is_using_external_context() const {
    return mUseExternalContext;
  }

  /**
   * @brief
   * Return a handle to the RHI API shared library
   *
   * @return
   * The handle to the RHI API shared library, can be a null pointer
   */
  [[nodiscard]] inline void *get_rhi_api_shared_library() const {
    return mRhiApiSharedLibrary;
  }

  /**
   * @brief
   * Set the handle for the RHI API shared library to use instead of let it load by the RHI instance
   *
   * @param[in] rhiApiSharedLibrary
   * A handle to the RHI API shared library, can be a null pointer; the RHI will use this handle instead of loading the RHI API shared library itself
   */
  inline void set_rhi_api_shared_library(void *rhiApiSharedLibrary) {
    mRhiApiSharedLibrary = rhiApiSharedLibrary;
  }

  // Private methods
private:
  explicit RHIContext(const RHIContext &) = delete;

  RHIContext &operator=(const RHIContext &) = delete;

  // Private data
protected:
  /** Handle to the native window */
  core::handle mNativeWindowHandle;
  /** A handle to the RHI API shared library (e.g. obtained via "dlopen()" and co), can be a null pointer */
  void *mRhiApiSharedLibrary;
  /** The platform data to communicate between frontend and rhi */
  core::FrontendPlatformData mPlatformData;
  /** Determines whether to use own or external context */
  bool mUseExternalContext;
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
