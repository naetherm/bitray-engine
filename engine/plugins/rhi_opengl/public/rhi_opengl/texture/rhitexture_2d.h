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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi_opengl {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract OpenGL 2D texture interface
 */
class RHITexture2D : public rhi::RHITexture2D {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Destructor
   */
  ~RHITexture2D() override;

  /**
   * @brief
   * Return the number of multisamples
   * 
   * @return
   * The number of multisamples per pixel (valid values: 1, 2, 4, 8)
   */
  [[nodiscard]] inline core::uint8 get_number_of_multisamples() const {
    return mNumberOfMultisamples;
  }

  /**
   * @brief
   * Return the OpenGL texture
   * 
   * @return
   * The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
   */
  [[nodiscard]] inline GLuint get_opengl_texture() const {
    return mOpenGLTexture;
  }

  /**
   * @brief
   * Return the OpenGL internal format
   * 
   * @return
   * The OpenGL internal format
   */
  [[nodiscard]] inline GLuint get_opengl_internal_format() const {
    return mOpenGLInternalFormat;
  }


  //[-------------------------------------------------------]
  //[ Public virtual Texture2D methods                      ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Set minimum maximum mipmap index
   * 
   * @param[in] minimumMipmapIndex
   * Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
   * @param[in] maximumMipmapIndex
   * Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
   */
  virtual void setMinimumMaximumMipmapIndex(core::uint32 minimumMipmapIndex, core::uint32 maximumMipmapIndex) = 0;


  //[-------------------------------------------------------]
  //[ Public virtual rhi::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual void *get_internal_resource_handle() const override
  {
    return reinterpret_cast<void *>(static_cast<uintptr_t>(mOpenGLTexture));
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override
  {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] openGLRhi
   * Owner OpenGL RHI instance
   * @param[in] width
   * The width of the texture
   * @param[in] height
   * The height of the texture
   * @param[in] textureFormat
   * Texture format
   * @param[in] numberOfMultisamples
   * The number of multisamples per pixel (valid values: 1, 2, 4, 8)
   */
  RHITexture2D(RHIDevice &openGLRhi,
               rhi::TextureDescriptor descriptor RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  core::uint8 mNumberOfMultisamples;  ///< The number of multisamples per pixel (valid values: 1, 2, 4, 8)
  GLuint mOpenGLTexture;      ///< OpenGL texture, can be zero if no resource is allocated
  GLuint mOpenGLInternalFormat;  ///< OpenGL internal format


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHITexture2D(const RHITexture2D &source) = delete;

  RHITexture2D &operator=(const RHITexture2D &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
