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
#include "rhi_opengl/rhi_opengl.h"
#include <rhi/texture/rhitexture_manager.h>
#include <rhi/texture/rhitexture_types.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;
class Extensions;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

/**
 * @brief
 * OpenGL texture manager interface
 */
class TextureManager final : public rhi::RHITextureManager {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   */
  explicit TextureManager(RHIDevice &openGLRhi);

  /**
   * @brief
   * Destructor
   */
  virtual ~TextureManager() override = default;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHITextureManager methods       ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual rhi::RHITexture1D *create_texture_1d(rhi::TextureDescriptor descriptor,
                                                               const void *data = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITexture1DArray *create_texture_1d_array(rhi::TextureDescriptor descriptor,
                                                                          const void *data = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITexture2D *
  create_texture_2d(rhi::TextureDescriptor descriptor, const void *data = nullptr,
                    [[maybe_unused]] const rhi::OptimizedTextureClearValue *optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITexture2DArray *create_texture_2d_array(rhi::TextureDescriptor descriptor,
                                                                          const void *data = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITexture3D *create_texture_3d(rhi::TextureDescriptor descriptor,
                                                               const void *data = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITextureCube *create_texture_cube(rhi::TextureDescriptor descriptor,
                                                                   const void *data = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual rhi::RHITextureCubeArray *create_texture_cube_array(rhi::TextureDescriptor descriptor,
                                                                              [[maybe_unused]] const void *data = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods             ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit TextureManager(const TextureManager &source) = delete;

  TextureManager &operator=(const TextureManager &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  const Extensions *mExtensions;  ///< Extensions instance, always valid
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
