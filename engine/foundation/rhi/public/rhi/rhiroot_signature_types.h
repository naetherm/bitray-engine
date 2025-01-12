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
#include "rhi/rhidynamicrhi_types.h"
#include "rhi/rhiresource_types.h"
#include "rhi/state/rhisampler_state_types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Descriptor range type
 *
 * @note
 * - These constants directly map to Direct3D 12 constants, do not change them
 *
 * @see
 * - "D3D12_DESCRIPTOR_RANGE_TYPE"-documentation for details
 */
enum class DescriptorRangeType {
  /// Shader resource view (SRV), in HLSL: "t<index>"-register
  SRV = 0,
  /// Unordered access view (UAV), in HLSL: "u<index>"-register
  UAV = SRV + 1,
  /// Uniform buffer view (UBV), in HLSL: "b<index>"-register, "UBV" = "CBV"; we're using the OpenGL/Vulkan terminology of "uniform buffer" instead of "constant buffer" as DirectX does
  UBV = UAV + 1,
  /// In HLSL: "s<index>"-register
  SAMPLER = UBV + 1,
  /// Number of range type, invalid descriptor range type
  NUMBER_OF_RANGE_TYPES = SAMPLER + 1  
};

/**
 * @brief
 * Shader visibility
 *
 * @note
 * - These constants directly map to Direct3D 12 constants, do not change them
 *
 * @see
 * - "D3D12_SHADER_VISIBILITY"-documentation for details
 */
enum class ShaderVisibility {
  ALL = 0,
  VERTEX = 1,
  TESSELLATION_CONTROL = 2,
  TESSELLATION_EVALUATION = 3,
  GEOMETRY = 4,
  FRAGMENT = 5,
  TASK = 6,
  MESH = 7,
  // The rest is not part of "D3D12_SHADER_VISIBILITY"
  COMPUTE = 8,
  ALL_GRAPHICS = 9
};

/**
 * @brief
 * Descriptor range
 *
 * @note
 * - "rhi::DescriptorRange" is not identical to "D3D12_DESCRIPTOR_RANGE" because it had to be extended by information required by OpenGL
 *
 * @see
 * - "D3D12_DESCRIPTOR_RANGE"-documentation for details
 */
struct DescriptorRange {
  DescriptorRangeType rangeType;
  core::uint32 numberOfDescriptors;
  core::uint32 baseShaderRegister;  /// When using explicit binding locations
  core::uint32 registerSpace;
  core::uint32 offsetInDescriptorsFromTableStart;

  // The rest is not part of "D3D12_DESCRIPTOR_RANGE" and was added to support Vulkan, OpenGL and Direct3D 9 as well
  static constexpr core::uint32 NAME_LENGTH = 31 + 1;  /// +1 for the terminating zero
  char baseShaderRegisterName[NAME_LENGTH];  /// When not using explicit binding locations (OpenGL ES 3, legacy GLSL profiles)
  ShaderVisibility shaderVisibility;
  ResourceType resourceType;
};

struct DescriptorRangeBuilder final : public DescriptorRange {
  static constexpr core::uint32 OFFSET_APPEND = 0xffffffff;

  static inline void initialize(
    DescriptorRange &range,
    ResourceType _resourceType,
    core::uint32 _baseShaderRegister,
    const char _baseShaderRegisterName[NAME_LENGTH],
    ShaderVisibility _shaderVisibility,
    DescriptorRangeType _rangeType = DescriptorRangeType::NUMBER_OF_RANGE_TYPES,  // Automatically determine the descriptor range type basing on the resource type
    core::uint32 _numberOfDescriptors = 1,
    core::uint32 _registerSpace = 0,
    core::uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND) {
    if (DescriptorRangeType::NUMBER_OF_RANGE_TYPES == _rangeType) {
      // Automatically determine the descriptor range type basing on the resource type
      switch (_resourceType) {
        case ResourceType::VERTEX_BUFFER:
        case ResourceType::INDEX_BUFFER:
        case ResourceType::TEXTURE_BUFFER:
        case ResourceType::STRUCTURED_BUFFER:
        case ResourceType::INDIRECT_BUFFER:
          range.rangeType = DescriptorRangeType::SRV;
          break;

        case ResourceType::UNIFORM_BUFFER:
          range.rangeType = DescriptorRangeType::UBV;
          break;

        case ResourceType::TEXTURE_1D:
        case ResourceType::TEXTURE_1D_ARRAY:
        case ResourceType::TEXTURE_2D:
        case ResourceType::TEXTURE_2D_ARRAY:
        case ResourceType::TEXTURE_3D:
        case ResourceType::TEXTURE_CUBE:
        case ResourceType::TEXTURE_CUBE_ARRAY:
          range.rangeType = DescriptorRangeType::SRV;
          break;

        case ResourceType::SAMPLER_STATE:
          range.rangeType = DescriptorRangeType::SAMPLER;
          break;

        case ResourceType::ROOT_SIGNATURE:
        case ResourceType::RESOURCE_GROUP:
        case ResourceType::GRAPHICS_PROGRAM:
        case ResourceType::VERTEX_ARRAY:
        case ResourceType::RENDER_PASS:
        case ResourceType::QUERY_POOL:
        case ResourceType::SWAP_CHAIN:
        case ResourceType::FRAMEBUFFER:
        case ResourceType::GRAPHICS_PIPELINE_STATE:
        case ResourceType::COMPUTE_PIPELINE_STATE:
        case ResourceType::VERTEX_SHADER:
        case ResourceType::TESSELLATION_CONTROL_SHADER:
        case ResourceType::TESSELLATION_EVALUATION_SHADER:
        case ResourceType::GEOMETRY_SHADER:
        case ResourceType::FRAGMENT_SHADER:
        case ResourceType::TASK_SHADER:
        case ResourceType::MESH_SHADER:
        case ResourceType::COMPUTE_SHADER:
          BE_ASSERT(false, "Invalid resource type")
          break;
      }
    } else {
      range.rangeType = _rangeType;
    }
    range.numberOfDescriptors = _numberOfDescriptors;
    range.baseShaderRegister = _baseShaderRegister;
    range.registerSpace = _registerSpace;
    range.offsetInDescriptorsFromTableStart = _offsetInDescriptorsFromTableStart;
    strcpy(range.baseShaderRegisterName, _baseShaderRegisterName);
    range.shaderVisibility = _shaderVisibility;
    range.resourceType = _resourceType;
    BE_ASSERT(ResourceType::UNIFORM_BUFFER != range.resourceType || DescriptorRangeType::UAV != range.rangeType,
           "Uniform buffer doesn't support UAV")
  }

  inline DescriptorRangeBuilder() {}

  inline explicit DescriptorRangeBuilder(const DescriptorRangeBuilder &) {}

  inline DescriptorRangeBuilder(
    ResourceType _resourceType,
    core::uint32 _baseShaderRegister,
    const char _baseShaderRegisterName[NAME_LENGTH],
    ShaderVisibility _shaderVisibility,
    DescriptorRangeType _rangeType = DescriptorRangeType::NUMBER_OF_RANGE_TYPES,  // Automatically determine the descriptor range type basing on the resource type
    core::uint32 _numberOfDescriptors = 1,
    core::uint32 _registerSpace = 0,
    core::uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND) {
    initialize(_resourceType, _baseShaderRegister, _baseShaderRegisterName, _shaderVisibility, _rangeType,
               _numberOfDescriptors, _registerSpace, _offsetInDescriptorsFromTableStart);
  }

  inline void initializeSampler(
    core::uint32 _baseShaderRegister,
    ShaderVisibility _shaderVisibility,
    core::uint32 _numberOfDescriptors = 1,
    core::uint32 _registerSpace = 0,
    core::uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND) {
    initialize(*this, ResourceType::SAMPLER_STATE, _baseShaderRegister, "", _shaderVisibility,
               DescriptorRangeType::SAMPLER, _numberOfDescriptors, _registerSpace, _offsetInDescriptorsFromTableStart);
  }

  inline void initialize(
    ResourceType _resourceType,
    core::uint32 _baseShaderRegister,
    const char _baseShaderRegisterName[NAME_LENGTH],
    ShaderVisibility _shaderVisibility,
    DescriptorRangeType _rangeType = DescriptorRangeType::NUMBER_OF_RANGE_TYPES,  // Automatically determine the descriptor range type basing on the resource type
    core::uint32 _numberOfDescriptors = 1,
    core::uint32 _registerSpace = 0,
    core::uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND) {
    initialize(*this, _resourceType, _baseShaderRegister, _baseShaderRegisterName, _shaderVisibility, _rangeType,
               _numberOfDescriptors, _registerSpace, _offsetInDescriptorsFromTableStart);
  }
};

/**
 * @brief
 * Root descriptor table
 *
 * @note
 * - This structure directly maps to Direct3D 12 structure, do not change it
 * - Samplers are not allowed in the same descriptor table as UBV/UAV/SRVs
 *
 * @see
 * - "D3D12_ROOT_DESCRIPTOR_TABLE"-documentation for details
 */
struct RootDescriptorTable {
  core::uint32 numberOfDescriptorRanges;
  core::uint64 descriptorRanges;      /// Can't use "const DescriptorRange*" because we need to have something platform neutral we can easily serialize without getting too fine granular
};

struct RootDescriptorTableBuilder final : public RootDescriptorTable {
  static inline void initialize(
    RootDescriptorTable &rootDescriptorTable,
    core::uint32 _numberOfDescriptorRanges,
    const DescriptorRange *_descriptorRanges) {
    rootDescriptorTable.numberOfDescriptorRanges = _numberOfDescriptorRanges;
    rootDescriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(_descriptorRanges);
  }

  inline RootDescriptorTableBuilder() {}

  inline explicit RootDescriptorTableBuilder(const RootDescriptorTableBuilder &) {}

  inline RootDescriptorTableBuilder(
    core::uint32 _numberOfDescriptorRanges,
    const DescriptorRange *_descriptorRanges) {
    initialize(_numberOfDescriptorRanges, _descriptorRanges);
  }

  inline void initialize(
    core::uint32 _numberOfDescriptorRanges,
    const DescriptorRange *_descriptorRanges) {
    initialize(*this, _numberOfDescriptorRanges, _descriptorRanges);
  }
};

/**
 * @brief
 * Root parameter type
 *
 * @note
 * - These constants directly map to Direct3D 12 constants, do not change them
 *
 * @see
 * - "D3D12_ROOT_PARAMETER_TYPE"-documentation for details
 */
enum class RootParameterType {
  DESCRIPTOR_TABLE = 0,
  CONSTANTS_32BIT = DESCRIPTOR_TABLE + 1,
  /// Uniform buffer view (UBV), in HLSL: "b<index>"-register, "UBV" = "CBV"; we're using the OpenGL/Vulkan terminology of "uniform buffer" instead of "constant buffer" as DirectX does
  UBV = CONSTANTS_32BIT + 1,
  /// Shader resource view (SRV), in HLSL: "t<index>"-register
  SRV = UBV + 1,
  /// Unordered access view (UAV), in HLSL: "u<index>"-register
  UAV = SRV + 1          
};

/**
 * @brief
 * Root constants
 *
 * @note
 * - This structure directly maps to Direct3D 12 structure, do not change it
 *
 * @see
 * - "D3D12_ROOT_CONSTANTS"-documentation for details
 */
struct RootConstants {
  core::uint32 shaderRegister;
  core::uint32 registerSpace;
  core::uint32 numberOf32BitValues;
};

struct RootConstantsBuilder final : public RootConstants {
  static inline void initialize(
    RootConstants &rootConstants,
    core::uint32 _numberOf32BitValues,
    core::uint32 _shaderRegister,
    core::uint32 _registerSpace = 0) {
    rootConstants.numberOf32BitValues = _numberOf32BitValues;
    rootConstants.shaderRegister = _shaderRegister;
    rootConstants.registerSpace = _registerSpace;
  }

  inline RootConstantsBuilder() {}

  inline explicit RootConstantsBuilder(const RootConstantsBuilder &) {}

  inline RootConstantsBuilder(
    core::uint32 _numberOf32BitValues,
    core::uint32 _shaderRegister,
    core::uint32 _registerSpace = 0) {
    initialize(_numberOf32BitValues, _shaderRegister, _registerSpace);
  }

  inline void initialize(
    core::uint32 _numberOf32BitValues,
    core::uint32 _shaderRegister,
    core::uint32 _registerSpace = 0) {
    initialize(*this, _numberOf32BitValues, _shaderRegister, _registerSpace);
  }
};

/**
 * @brief
 * Root descriptor
 *
 * @note
 * - This structure directly maps to Direct3D 12 structure, do not change it
 *
 * @see
 * - "D3D12_ROOT_DESCRIPTOR"-documentation for details
 */
struct RootDescriptor {
  core::uint32 shaderRegister;
  core::uint32 registerSpace;
};

struct RootDescriptorBuilder final : public RootDescriptor {
  static inline void
  initialize(RootDescriptor &table, core::uint32 _shaderRegister, core::uint32 _registerSpace = 0) {
    table.shaderRegister = _shaderRegister;
    table.registerSpace = _registerSpace;
  }

  inline RootDescriptorBuilder() {}

  inline explicit RootDescriptorBuilder(const RootDescriptorBuilder &) {}

  inline RootDescriptorBuilder(
    core::uint32 _shaderRegister,
    core::uint32 _registerSpace = 0) {
    initialize(_shaderRegister, _registerSpace);
  }

  inline void initialize(
    core::uint32 _shaderRegister,
    core::uint32 _registerSpace = 0) {
    initialize(*this, _shaderRegister, _registerSpace);
  }
};

/**
 * @brief
 * Root parameter
 *
 * @note
 * - "rhi::RootParameter" is not identical to "D3D12_ROOT_PARAMETER", the shader visibility is defined per descriptor since Vulkan needs it this way
 *
 * @see
 * - "D3D12_ROOT_PARAMETER"-documentation for details
 */
struct RootParameter {
  RootParameterType parameterType;
  union {
    RootDescriptorTable descriptorTable;
    RootConstants constants;
    RootDescriptor descriptor;
  };
};
struct RootParameterData final {
  RootParameterType parameterType;
  core::uint32 numberOfDescriptorRanges;
};

struct RootParameterBuilder final : public RootParameter {
  static inline void initializeAsDescriptorTable(
    RootParameter &rootParam,
    core::uint32 numberOfDescriptorRanges,
    const DescriptorRange *descriptorRanges) {
    rootParam.parameterType = RootParameterType::DESCRIPTOR_TABLE;
    RootDescriptorTableBuilder::initialize(rootParam.descriptorTable, numberOfDescriptorRanges, descriptorRanges);
  }

  static inline void initializeAsConstants(
    RootParameter &rootParam,
    core::uint32 numberOf32BitValues,
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    rootParam.parameterType = RootParameterType::CONSTANTS_32BIT;
    RootConstantsBuilder::initialize(rootParam.constants, numberOf32BitValues, shaderRegister, registerSpace);
  }

  static inline void initializeAsConstantBufferView(
    RootParameter &rootParam,
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    rootParam.parameterType = RootParameterType::UBV;
    RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
  }

  static inline void initializeAsShaderResourceView(
    RootParameter &rootParam,
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    rootParam.parameterType = RootParameterType::SRV;
    RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
  }

  static inline void initializeAsUnorderedAccessView(
    RootParameter &rootParam,
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    rootParam.parameterType = RootParameterType::UAV;
    RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
  }

  inline RootParameterBuilder() {}

  inline explicit RootParameterBuilder(const RootParameterBuilder &) {}

  inline void initializeAsDescriptorTable(
    core::uint32 numberOfDescriptorRanges,
    const DescriptorRange *descriptorRanges) {
    initializeAsDescriptorTable(*this, numberOfDescriptorRanges, descriptorRanges);
  }

  inline void initializeAsConstants(
    core::uint32 numberOf32BitValues,
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    initializeAsConstants(*this, numberOf32BitValues, shaderRegister, registerSpace);
  }

  inline void initializeAsConstantBufferView(
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    initializeAsConstantBufferView(*this, shaderRegister, registerSpace);
  }

  inline void initializeAsShaderResourceView(
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    initializeAsShaderResourceView(*this, shaderRegister, registerSpace);
  }

  inline void initializeAsUnorderedAccessView(
    core::uint32 shaderRegister,
    core::uint32 registerSpace = 0) {
    initializeAsUnorderedAccessView(*this, shaderRegister, registerSpace);
  }
};

/**
 * @brief
 * Root signature flags
 *
 * @note
 * - These constants directly map to Direct3D 12 constants, do not change them
 *
 * @see
 * - "D3D12_ROOT_SIGNATURE_FLAGS"-documentation for details
 */
struct RootSignatureFlags final {
  enum Enum {
    NONE = 0,
    ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT = 0x1,
    DENY_VERTEX_SHADER_ROOT_ACCESS = 0x2,
    DENY_TESSELLATION_CONTROL_SHADER_ROOT_ACCESS = 0x4,
    DENY_TESSELLATION_EVALUATION_SHADER_ROOT_ACCESS = 0x8,
    DENY_GEOMETRY_SHADER_ROOT_ACCESS = 0x10,
    DENY_FRAGMENT_SHADER_ROOT_ACCESS = 0x20,
    ALLOW_STREAM_OUTPUT = 0x40
  };
};

/**
 * @brief
 * Static border color
 *
 * @note
 * - These constants directly map to Direct3D 12 constants, do not change them
 *
 * @see
 * - "D3D12_STATIC_BORDER_COLOR"-documentation for details
 */
enum class StaticBorderColor {
  TRANSPARENT_BLACK = 0,
  OPAQUE_BLACK = TRANSPARENT_BLACK + 1,
  OPAQUE_WHITE = OPAQUE_BLACK + 1
};

/**
 * @brief
 * Static sampler
 *
 * @note
 * - This structure directly maps to Direct3D 12 structure, do not change it
 *
 * @see
 * - "D3D12_STATIC_SAMPLER_DESC"-documentation for details
 */
struct StaticSampler final {
  FilterMode filter;
  TextureAddressMode addressU;
  TextureAddressMode addressV;
  TextureAddressMode addressW;
  float mipLodBias;
  core::uint32 maxAnisotropy;
  ComparisonFunc comparisonFunc;
  StaticBorderColor borderColor;
  float minLod;
  float maxLod;
  core::uint32 shaderRegister;
  core::uint32 registerSpace;
  ShaderVisibility shaderVisibility;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
