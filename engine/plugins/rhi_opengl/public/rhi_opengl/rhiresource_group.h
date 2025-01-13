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
#include <rhi/rhiresource_group.h>


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
 * OpenGL resource group class
 */
class RHIResourceGroup final : public rhi::RHIResourceGroup {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   * 
   * @param[in] rhi
   * Owner RHI instance
   * @param[in] rootSignature
   * Root signature
   * @param[in] rootParameterIndex
   * The root parameter index number for binding
   * @param[in] numberOfResources
   * Number of resources, having no resources is invalid
   * @param[in] resources
   * At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
   * @param[in] samplerStates
   * If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
   */
  RHIResourceGroup(rhi::RHIDevice &rhi, const rhi::RootSignatureDescriptor &rootSignature,
                   core::uint32 rootParameterIndex, core::uint32 numberOfResources, rhi::RHIResource **resources,
                   rhi::RHISamplerState **samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
   * @brief
   * Destructor
   */
  virtual ~RHIResourceGroup() override;

  /**
   * @brief
   * Return the number of resources this resource group groups together
   * 
   * @return
   * The number of resources this resource group groups together
   */
  [[nodiscard]] inline core::uint32 get_number_of_resources() const {
    return mNumberOfResources;
  }

  /**
   * @brief
   * Return the RHI resources
   * 
   * @return
   * The RHI resources, don't release or destroy the returned pointer
   */
  [[nodiscard]] inline rhi::RHIResource **get_resources() const {
    return mResources;
  }

  /**
   * @brief
   * Return the sampler states
   * 
   * @return
   * The sampler states, don't release or destroy the returned pointer
   */
  [[nodiscard]] inline rhi::RHISamplerState **get_sampler_state() const {
    return mSamplerStates;
  }

  /**
   * @brief
   * Return the resource index to uniform block binding index mapping
   * 
   * @return
   * The resource index to uniform block binding index mapping, only valid for uniform buffer resources
   */
  [[nodiscard]] inline core::uint32 *get_resource_index_to_uniform_block_binding_index() const {
    return mResourceIndexToUniformBlockBindingIndex;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual rhi::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void self_destruct() override {
    re_delete(this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIResourceGroup(const RHIResourceGroup &source) = delete;

  RHIResourceGroup &operator=(const RHIResourceGroup &source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  core::uint32 mRootParameterIndex;            ///< The root parameter index number for binding
  core::uint32 mNumberOfResources;            ///< Number of resources this resource group groups together
  rhi::RHIResource **mResources;                ///< RHI resources, we keep a reference to it
  rhi::RHISamplerState **mSamplerStates;              ///< Sampler states, we keep a reference to it
  core::uint32 *mResourceIndexToUniformBlockBindingIndex;  ///< Resource index to uniform block binding index mapping, only valid for uniform buffer resources


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi_opengl
