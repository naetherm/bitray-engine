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
 * Abstract OpenGL 2D array texture interface
 */
class RHITexture2DArray : public rhi::RHITexture2DArray {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Destructor
   */
  ~RHITexture2DArray() override;

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
   * @param[in] numberOfSlices
   * The number of slices
   * @param[in] textureFormat
   * Texture format
   */
  RHITexture2DArray(RHIDevice &openGLRhi,
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
  explicit RHITexture2DArray(const RHITexture2DArray &source) = delete;

  RHITexture2DArray &operator=(const RHITexture2DArray &source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
