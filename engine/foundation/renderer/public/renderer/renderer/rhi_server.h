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
#include "renderer/renderer.h"
#include <core/core/server_impl.h>
#include <core/core/ptr.h>
#include <core/platform/library.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace rhi {
class RHIDevice;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace renderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * RhiServer
 *
 * @brief
 * Rhi server.
 */
class RhiServer : public core::ServerImpl {
public:
  /**
   * @brief
   * Constructor.
   */
  RhiServer();

  /**
   * @brief
   * Destructor.
   */
  ~RhiServer() override;


  /**
   * @brief
   * Initializes the RHI server with a specified RHI name and context.
   *
   * @param rhiName
   * The name of the RHI backend to load (e.g., "DirectX11", "Vulkan").
   *
   * @param rhiContext
   * A pointer to the RHI context used for backend initialization.
   */
  void initialize(const core::String& rhiName, rhi::RHIContext* rhiContext);

  /**
   * @brief
   * Initializes the RHI server with a specific RHI device.
   *
   * @param rhiDevice
   * A pointer to the RHI device used for backend operations.
   */
  void initialize(rhi::RHIDevice* rhiDevice);

  /**
   * @brief
   * Retrieves the RHI device associated with the server.
   *
   * @return
   * A constant pointer to the RHI device.
   */
  [[nodiscard]] inline const rhi::RHIDevice* get_rhi_device() const;

  /**
   * @brief
   * Retrieves the RHI device associated with the server.
   *
   * @return
   * A pointer to the RHI device.
   */
  [[nodiscard]] inline rhi::RHIDevice* get_rhi_device();

private:

  /**
   * @brief
   * Creates an RHI device based on the specified RHI name and context.
   *
   * This method is called internally during initialization to set up the RHI device.
   */
  void create_rhi_device();

private:
  /** The name of the rhi backend to load */
  core::String mRHIName;
  /** Pointer to the dynamic library of the rhi backend */
  core::Library* mRhiDynLib;
  /** Pointer to the rhi context */
  rhi::RHIContext* mRHIContext;
  /** Pointer to the rhi device */
  rhi::RHIDevice* mRHIDevice;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "renderer/renderer/rhi_server.inl"