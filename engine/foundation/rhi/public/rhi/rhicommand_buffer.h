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
#include "rhi/rhidynamicrhi.h"
#include "rhi/rhiresource.h"
#include "rhi/rhiroot_signature.h"
#include "rhi/rhiresource_group.h"
#include "rhi/buffer/rhiindex_buffer_types.h"
#include "rhi/buffer/rhiindirect_buffer_types.h"
#include "rhi/state/rhisampler_state.h"
#include "rhi/state/rhicompute_pipeline_state.h"
#include "rhi/state/rhigraphics_pipeline_state.h"
#include "rhi/query/rhiquery_pool.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RHIVertexArray;
class RHIRenderTarget;
class RHIIndirectBuffer;
class RHIComputePipelineState;
class RHITexture;
class RHIFramebuffer;
class RHIUniformBuffer;
class RHIGraphicsProgram;
class RHIQueryPool;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
enum class CommandDispatchFunctionIndex : core::uint8 {
  // Command buffer
  DISPATCH_COMMAND_BUFFER = 0,
  // Graphics
  SET_GRAPHICS_ROOT_SIGNATURE,
  SET_GRAPHICS_PIPELINE_STATE,
  SET_GRAPHICS_RESOURCE_GROUP,
  SET_GRAPHICS_VERTEX_ARRAY,      // Input-assembler (IA) stage
  SET_GRAPHICS_VIEWPORTS,        // Rasterizer (RS) stage
  SET_GRAPHICS_SCISSOR_RECTANGLES,  // Rasterizer (RS) stage
  SET_GRAPHICS_RENDER_TARGET,      // Output-merger (OM) stage
  CLEAR_GRAPHICS,
  DRAW_GRAPHICS,
  DRAW_INDEXED_GRAPHICS,
  DRAW_MESH_TASKS,
  // Compute
  SET_COMPUTE_ROOT_SIGNATURE,
  SET_COMPUTE_PIPELINE_STATE,
  SET_COMPUTE_RESOURCE_GROUP,
  DISPATCH_COMPUTE,
  // Resource
  SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX,
  RESOLVE_MULTISAMPLE_FRAMEBUFFER,
  COPY_RESOURCE,
  GENERATE_MIPMAPS,
  COPY_UNIFORM_BUFFER_DATA,
  SET_UNIFORM,
  // Query
  RESET_QUERY_POOL,
  BEGIN_QUERY,
  END_QUERY,
  WRITE_TIMESTAMP_QUERY,
  // Debug
  SET_DEBUG_MARKER,
  BEGIN_DEBUG_EVENT,
  END_DEBUG_EVENT,
  // Done
  NUMBER_OF_FUNCTIONS
};

typedef void (*ImplementationDispatchFunction)(const void *, RHIDevice &rhi);

typedef void *CommandPacket;
typedef const void *ConstCommandPacket;

// Global functions
namespace CommandPacketHelper {
static constexpr core::uint32 OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX = 0u;
static constexpr core::uint32 OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX + sizeof(core::uint32);
static constexpr core::uint32 OFFSET_COMMAND = OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION + sizeof(core::uint32);  // Don't use "sizeof(CommandDispatchFunctionIndex)" instead of "sizeof(core::uint32)" so we have a known alignment

template<typename T>
[[nodiscard]] inline core::uint32 getNumberOfBytes(core::uint32 numberOfAuxiliaryBytes) {
  return OFFSET_COMMAND + sizeof(T) + numberOfAuxiliaryBytes;
}

[[nodiscard]] inline core::uint32 getNextCommandPacketByteIndex(const CommandPacket commandPacket) {
  return *reinterpret_cast<const core::uint32 *>(reinterpret_cast<const core::uint8 *>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
}

[[nodiscard]] inline core::uint32 getNextCommandPacketByteIndex(const ConstCommandPacket constCommandPacket) {
  return *reinterpret_cast<const core::uint32 *>(reinterpret_cast<const core::uint8 *>(constCommandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX);
}

inline void storeNextCommandPacketByteIndex(const CommandPacket commandPacket, core::uint32 nextPacketByteIndex) {
  *reinterpret_cast<core::uint32 *>(reinterpret_cast<core::uint8 *>(commandPacket) + OFFSET_NEXT_COMMAND_PACKET_BYTE_INDEX) = nextPacketByteIndex;
}

[[nodiscard]] inline CommandDispatchFunctionIndex *getCommandDispatchFunctionIndex(const CommandPacket commandPacket) {
  return reinterpret_cast<CommandDispatchFunctionIndex *>(reinterpret_cast<core::uint8 *>(commandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
}

[[nodiscard]] inline const CommandDispatchFunctionIndex* getCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket) {
  return reinterpret_cast<const CommandDispatchFunctionIndex *>(
    reinterpret_cast<const core::uint8 *>(constCommandPacket) + OFFSET_IMPLEMENTATION_DISPATCH_FUNCTION);
}

inline void storeImplementationDispatchFunctionIndex(const CommandPacket commandPacket, CommandDispatchFunctionIndex commandDispatchFunctionIndex) {
  *getCommandDispatchFunctionIndex(commandPacket) = commandDispatchFunctionIndex;
}

[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const CommandPacket commandPacket) {
  return *getCommandDispatchFunctionIndex(commandPacket);
}

[[nodiscard]] inline CommandDispatchFunctionIndex loadCommandDispatchFunctionIndex(const ConstCommandPacket constCommandPacket) {
  return *getCommandDispatchFunctionIndex(constCommandPacket);
}

template<typename T>
[[nodiscard]] inline T *getCommand(const CommandPacket commandPacket) {
  return reinterpret_cast<T *>(reinterpret_cast<core::uint8 *>(commandPacket) + OFFSET_COMMAND);
}

[[nodiscard]] inline const void *loadCommand(const CommandPacket commandPacket) {
  return reinterpret_cast<core::uint8 *>(commandPacket) + OFFSET_COMMAND;
}

[[nodiscard]] inline const void *loadCommand(const ConstCommandPacket constCommandPacket) {
  return reinterpret_cast<const core::uint8 *>(constCommandPacket) + OFFSET_COMMAND;
}

/**
 * @brief
 * Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
 */
template<typename T>
[[nodiscard]] inline core::uint8 *getAuxiliaryMemory(T *command) {
  return reinterpret_cast<core::uint8 *>(command) + sizeof(T);
}

/**
 * @brief
 * Return auxiliary memory address of the given command; returned memory address is considered unstable and might change as soon as another command is added
 */
template<typename T>
[[nodiscard]] inline const core::uint8 *getAuxiliaryMemory(const T *command) {
  return reinterpret_cast<const core::uint8 *>(command) + sizeof(T);
}

}

/**
 * @brief
 * Command buffer
 *
 * @remarks
 * Basing on
 * - "Molecular Musings" - "Stateless, layered, multi-threaded rendering � Part 1" by Stefan Reinalter from November 6, 2014 - https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
 * - "Molecular Musings" - "Stateless, layered, multi-threaded rendering � Part 2" by Stefan Reinalter from November 13, 2014 - http://molecularmusings.wordpress.com/2014/11/13/stateless-layered-multi-threaded-rendering-part-2-stateless-api-design/
 * - "Molecular Musings" - "Stateless, layered, multi-threaded rendering � Part 3" by Stefan Reinalter from December 16, 2014 - http://molecularmusings.wordpress.com/2014/12/16/stateless-layered-multi-threaded-rendering-part-3-api-design-details/
 * - "realtimecollisiondetection.net � the blog" - "Order your graphics draw calls around!" by Christer Ericson from October 3, 2008 - http://realtimecollisiondetection.net/blog/?p=86
 * but without a key inside the more general command buffer. Sorting is a job of a more high level construct like a render queue which also automatically will perform
 * batching and instancing. Also the memory management is much simplified to be cache friendly.
 *
 * @note
 * - The commands are stored as a flat contiguous array to be cache friendly
 * - Each command can have an additional auxiliary buffer, e.g. to store uniform buffer data to dispatch to the RHI
 * - It's valid to record a command buffer only once, and dispatch it multiple times to the RHI
 */
class RHICommandBuffer final {

  // Public methods
public:
  /**
   * @brief
   * Default constructor
   */
  inline RHICommandBuffer()
  : mCommandPacketBufferNumberOfBytes(0)
  , mCommandPacketBuffer(nullptr)
  , mPreviousCommandPacketByteIndex(~0u)
  , mCurrentCommandPacketByteIndex(0)
#ifdef DEBUG
  , mNumberOfCommands(0)
#endif
  {}

  /**
   * @brief
   * Destructor
   */
  inline ~RHICommandBuffer() {
    delete[] mCommandPacketBuffer;
  }

  /**
   * @brief
   * Return whether or not the command buffer is empty
   *
   * @return
   * "true" if the command buffer is empty, else "false"
   */
  [[nodiscard]] inline bool is_empty() const {
    return (~0u == mPreviousCommandPacketByteIndex);
  }

#ifdef DEBUG

  /**
   * @brief
   * Return the number of commands inside the command buffer
   *
   * @return
   * The number of commands inside the command buffer
   *
   * @note
   * - Counting the number of commands inside the command buffer is only a debugging feature and not used in optimized builds
   */
  [[nodiscard]] inline core::uint32 get_number_of_commands() const {
    return mNumberOfCommands;
  }

#endif

  /**
   * @brief
   * Return the command packet buffer
   *
   * @return
   * The command packet buffer, can be a null pointer, don't destroy the instance
   *
   * @note
   * - Internal, don't access the method if you don't have to
   */
  [[nodiscard]] inline const core::uint8 *get_command_packet_buffer() const {
    return (~0u != mPreviousCommandPacketByteIndex) ? mCommandPacketBuffer : nullptr;
  }

  /**
   * @brief
   * Clear the command buffer
   */
  inline void clear() {
    mPreviousCommandPacketByteIndex = ~0u;
    mCurrentCommandPacketByteIndex = 0;
#ifdef DEBUG
    mNumberOfCommands = 0;
#endif
  }

  /**
   * @brief
   * Add command
   *
   * @param[in] numberOfAuxiliaryBytes
   * Optional number of auxiliary bytes, e.g. to store uniform buffer data to dispatch to the RHI
   *
   * @return
   * Pointer to the added command, only null pointer in case of apocalypse, don't destroy the memory
   */
  template<typename U>
  [[nodiscard]] U *add_command(core::uint32 numberOfAuxiliaryBytes = 0) {
    // How many command package buffer bytes are consumed by the command to add?
    const core::uint32 numberOfCommandBytes = CommandPacketHelper::getNumberOfBytes<U>(numberOfAuxiliaryBytes);

    // 4294967295 is the maximum value of an "core::uint32"-type: Check for overflow
    // -> We use the magic number here to avoid "std::numeric_limits::max()" usage
    BE_ASSERT((static_cast<core::uint64>(mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index")

    // Grow command packet buffer, if required
    if (mCommandPacketBufferNumberOfBytes < mCurrentCommandPacketByteIndex + numberOfCommandBytes) {
      // Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
      const core::uint32 newCommandPacketBufferNumberOfBytes = mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
      core::uint8 *newCommandPacketBuffer = new core::uint8[newCommandPacketBufferNumberOfBytes];

      // Copy over current command package buffer content and free it, if required
      if (nullptr != mCommandPacketBuffer) {
        memcpy(newCommandPacketBuffer, mCommandPacketBuffer, mCommandPacketBufferNumberOfBytes);
        delete[] mCommandPacketBuffer;
      }

      // Finalize
      mCommandPacketBuffer = newCommandPacketBuffer;
      mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
    }

    // get command package for the new command
    CommandPacket commandPacket = &mCommandPacketBuffer[mCurrentCommandPacketByteIndex];

    // Setup previous and current command package
    if (~0u != mPreviousCommandPacketByteIndex) {
      CommandPacketHelper::storeNextCommandPacketByteIndex(&mCommandPacketBuffer[mPreviousCommandPacketByteIndex], mCurrentCommandPacketByteIndex);
    }
    CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, ~0u);
    CommandPacketHelper::storeImplementationDispatchFunctionIndex(commandPacket, U::COMMAND_DISPATCH_FUNCTION_INDEX);
    mPreviousCommandPacketByteIndex = mCurrentCommandPacketByteIndex;
    mCurrentCommandPacketByteIndex += numberOfCommandBytes;

    // Done
#ifdef DEBUG
    ++mNumberOfCommands;
#endif
    return CommandPacketHelper::getCommand<U>(commandPacket);
  }

  /**
   * @brief
   * Dispatch the command buffer to the RHI without flushing; use this for recording command buffers once and dispatch them multiple times
   *
   * @param[in] rhi
   * RHI to dispatch the command buffer to
   */
  inline void dispatch_to_rhi(RHIDevice &rhi) const {
    rhi.dispatch_command_buffer(*this);
  }

  /**
   * @brief
   * Dispatch the command buffer to the RHI and clear so the command buffer is empty again
   *
   * @param[in] rhi
   * RHI to dispatch the command buffer to
   */
  inline void dispatch_to_rhi_and_clear(RHIDevice &rhi) {
    rhi.dispatch_command_buffer(*this);
    clear();
  }

  /**
   * @brief
   * Append the command buffer to another command buffer without flushing; use this for recording command buffers once and append them multiple times
   *
   * @param[in] commandBuffer
   * Command buffer to append the command buffer to
   *
   * @note
   * - Use "rhi::Command::dispatch_command_buffer" to dispatch a command buffer inside another command buffer instead of appending it
   */
  inline void append_to_command_buffer(RHICommandBuffer &commandBuffer) const {
    // Sanity checks
    BE_ASSERT(this != &commandBuffer, "Can't append a command buffer to itself")
    BE_ASSERT(!is_empty(), "Can't append empty command buffers")

    // How many command package buffer bytes are consumed by the command to add?
    const core::uint32 numberOfCommandBytes = mCurrentCommandPacketByteIndex;

    // 4294967295 is the maximum value of an "core::uint32"-type: Check for overflow
    // -> We use the magic number here to avoid "std::numeric_limits::max()" usage
    BE_ASSERT((static_cast<core::uint64>(commandBuffer.mCurrentCommandPacketByteIndex) + numberOfCommandBytes) < 4294967295u, "Invalid current command packet byte index")

    // Grow command packet buffer, if required
    if (commandBuffer.mCommandPacketBufferNumberOfBytes < commandBuffer.mCurrentCommandPacketByteIndex + numberOfCommandBytes) {
      // Allocate new memory, grow using a known value but do also add the number of bytes consumed by the current command to add (many auxiliary bytes might be requested)
      const core::uint32 newCommandPacketBufferNumberOfBytes = commandBuffer.mCommandPacketBufferNumberOfBytes + NUMBER_OF_BYTES_TO_GROW + numberOfCommandBytes;
      core::uint8 *newCommandPacketBuffer = new core::uint8[newCommandPacketBufferNumberOfBytes];

      // Copy over current command package buffer content and free it, if required
      if (nullptr != commandBuffer.mCommandPacketBuffer) {
        memcpy(newCommandPacketBuffer, commandBuffer.mCommandPacketBuffer, commandBuffer.mCommandPacketBufferNumberOfBytes);
        delete[] commandBuffer.mCommandPacketBuffer;
      }

      // Finalize
      commandBuffer.mCommandPacketBuffer = newCommandPacketBuffer;
      commandBuffer.mCommandPacketBufferNumberOfBytes = newCommandPacketBufferNumberOfBytes;
    }

    // Copy over the command buffer in one burst
    memcpy(&commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex], mCommandPacketBuffer, mCurrentCommandPacketByteIndex);

    // Setup previous command package
    if (~0u != commandBuffer.mPreviousCommandPacketByteIndex) {
      CommandPacketHelper::storeNextCommandPacketByteIndex(&commandBuffer.mCommandPacketBuffer[commandBuffer.mPreviousCommandPacketByteIndex], commandBuffer.mCurrentCommandPacketByteIndex);
    }

    // Update command package indices
    CommandPacket commandPacket = &commandBuffer.mCommandPacketBuffer[commandBuffer.mCurrentCommandPacketByteIndex];
    core::uint32 nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
    while (~0u != nextCommandPacketByteIndex) {
      nextCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + nextCommandPacketByteIndex;
      CommandPacketHelper::storeNextCommandPacketByteIndex(commandPacket, nextCommandPacketByteIndex);
      commandPacket = &commandBuffer.mCommandPacketBuffer[nextCommandPacketByteIndex];
      nextCommandPacketByteIndex = CommandPacketHelper::getNextCommandPacketByteIndex(commandPacket);
    }

    // Finalize
    commandBuffer.mPreviousCommandPacketByteIndex = commandBuffer.mCurrentCommandPacketByteIndex + mPreviousCommandPacketByteIndex;
    commandBuffer.mCurrentCommandPacketByteIndex += mCurrentCommandPacketByteIndex;
#ifdef DEBUG
    commandBuffer.mNumberOfCommands += mNumberOfCommands;
#endif
  }

  /**
   * @brief
   * Append the command buffer to another command buffer and clear so the command buffer is empty again
   *
   * @param[in] commandBuffer
   * Command buffer to append the command buffer to
   */
  inline void append_to_command_buffer_and_clear(RHICommandBuffer &commandBuffer) {
    append_to_command_buffer(commandBuffer);
    clear();
  }

  // Private definitions
private:
  static constexpr core::uint32 NUMBER_OF_BYTES_TO_GROW = 8192;

  // Private data
private:
  // Memory
  core::uint32 mCommandPacketBufferNumberOfBytes;
  core::uint8 *mCommandPacketBuffer;
  // Current state
  core::uint32 mPreviousCommandPacketByteIndex;
  core::uint32 mCurrentCommandPacketByteIndex;
#ifdef DEBUG
  core::uint32 mNumberOfCommands;
#endif

};

// Concrete commands
namespace Command {

//[-------------------------------------------------------]
//[ Command buffer                                        ]
//[-------------------------------------------------------]
/**
 * @brief
 * Dispatch command buffer
 *
 * @note
 * - Use "rhi::CommandBuffer::append_to_command_buffer()" to append a command buffer to another command buffer instead of dispatching it
 */
struct DispatchCommandBuffer final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHICommandBuffer *commandBufferToDispatch) {
    BE_ASSERT(nullptr != commandBufferToDispatch, "Invalid command buffer to dispatch")
    *commandBuffer.add_command<DispatchCommandBuffer>() = DispatchCommandBuffer(commandBufferToDispatch);
  }

  // Constructor
  inline explicit DispatchCommandBuffer(RHICommandBuffer *_commandBufferToDispatch) :
    commandBufferToDispatch(_commandBufferToDispatch) {}

  // Data
  RHICommandBuffer *commandBufferToDispatch;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DISPATCH_COMMAND_BUFFER;
};

//[-------------------------------------------------------]
//[ Graphics                                              ]
//[-------------------------------------------------------]
/**
 * @brief
 * Set the used graphics root signature
 *
 * @param[in] rootSignature
 * Graphics root signature to use, can be an null pointer (default: "nullptr")
 */
struct SetGraphicsRootSignature final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIRootSignature *rootSignature) {
    *commandBuffer.add_command<SetGraphicsRootSignature>() = SetGraphicsRootSignature(rootSignature);
  }

  // Constructor
  inline explicit SetGraphicsRootSignature(RHIRootSignature *_rootSignature) :
    rootSignature(_rootSignature) {}

  // Data
  RHIRootSignature *rootSignature;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_ROOT_SIGNATURE;
};

/**
 * @brief
 * Set the used graphics pipeline state
 *
 * @param[in] graphicsPipelineState
 * Graphics pipeline state to use, can be an null pointer (default: "nullptr")
 */
struct SetGraphicsPipelineState final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIGraphicsPipelineState *graphicsPipelineState) {
    *commandBuffer.add_command<SetGraphicsPipelineState>() = SetGraphicsPipelineState(graphicsPipelineState);
  }

  // Constructor
  inline explicit SetGraphicsPipelineState(RHIGraphicsPipelineState *_graphicsPipelineState) :
    graphicsPipelineState(_graphicsPipelineState) {}

  // Data
  RHIGraphicsPipelineState *graphicsPipelineState;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_PIPELINE_STATE;
};

/**
 * @brief
 * Set a graphics resource group
 *
 * @param[in] rootParameterIndex
 * The root parameter index number for binding
 * @param[in] resourceGroup
 * Resource group to set
 */
struct SetGraphicsResourceGroup final {
  // Static methods
  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 rootParameterIndex, RHIResourceGroup *resourceGroup) {
    *commandBuffer.add_command<SetGraphicsResourceGroup>() = SetGraphicsResourceGroup(rootParameterIndex,
                                                                                      resourceGroup);
  }

  // Constructor
  inline SetGraphicsResourceGroup(core::uint32 _rootParameterIndex, RHIResourceGroup *_resourceGroup) :
    rootParameterIndex(_rootParameterIndex),
    resourceGroup(_resourceGroup) {}

  // Data
  core::uint32 rootParameterIndex;
  RHIResourceGroup *resourceGroup;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RESOURCE_GROUP;
};

/**
 * @brief
 * Set the used vertex array, input-assembler (IA) stage
 *
 * @param[in] vertexArray
 * Vertex array to use, can be an null pointer (default: "nullptr")
 */
struct SetGraphicsVertexArray final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIVertexArray *vertexArray) {
    *commandBuffer.add_command<SetGraphicsVertexArray>() = SetGraphicsVertexArray(vertexArray);
  }

  // Constructor
  inline explicit SetGraphicsVertexArray(RHIVertexArray *_vertexArray) :
    vertexArray(_vertexArray) {}

  // Data
  RHIVertexArray *vertexArray;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VERTEX_ARRAY;
};

/**
 * @brief
 * Set the graphics viewports, rasterizer (RS) stage
 *
 * @param[in] numberOfViewports
 * Number of viewports, if <1 nothing happens, must be <="rhi::Capabilities::maximumNumberOfViewports"
 * @param[in] viewports
 * C-array of viewports, there must be at least "numberOfViewports"-viewports, in case of a null pointer nothing happens
 *
 * @note
 * - The current viewport(s) does not affect the clear operation
 * - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
 * Use the convenience "rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
 */
struct SetGraphicsViewports final {
  // Static methods
  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 numberOfViewports, const Viewport *viewports) {
    *commandBuffer.add_command<SetGraphicsViewports>() = SetGraphicsViewports(numberOfViewports, viewports);
  }

  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 topLeftX, core::uint32 topLeftY, core::uint32 width,
         core::uint32 height, float minimumDepth = 0.0f, float maximumDepth = 1.0f) {
    SetGraphicsViewports *setGraphicsViewportsCommand = commandBuffer.add_command<SetGraphicsViewports>(
      sizeof(Viewport));

    // Set command data
    Viewport *viewport = reinterpret_cast<Viewport *>(CommandPacketHelper::getAuxiliaryMemory(
      setGraphicsViewportsCommand));
    viewport->topLeftX = static_cast<float>(topLeftX);
    viewport->topLeftY = static_cast<float>(topLeftY);
    viewport->width = static_cast<float>(width);
    viewport->height = static_cast<float>(height);
    viewport->minDepth = minimumDepth;
    viewport->maxDepth = maximumDepth;

    // Finalize command
    setGraphicsViewportsCommand->numberOfViewports = 1;
    setGraphicsViewportsCommand->viewports = nullptr;
  }

  // Constructor
  inline SetGraphicsViewports(core::uint32 _numberOfViewports, const Viewport *_viewports) :
    numberOfViewports(_numberOfViewports),
    viewports(_viewports) {}

  // Data
  core::uint32 numberOfViewports;
  const Viewport *viewports;  ///< If null pointer, command auxiliary memory is used instead
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_VIEWPORTS;
};

/**
 * @brief
 * Set the graphics scissor rectangles, rasterizer (RS) stage
 *
 * @param[in] numberOfScissorRectangles
 * Number of scissor rectangles, if <1 nothing happens, must be <="rhi::Capabilities::maximumNumberOfViewports"
 * @param[in] dcissorRectangles
 * C-array of scissor rectangles, there must be at least "numberOfScissorRectangles" scissor rectangles, in case of a null pointer nothing happens
 *
 * @note
 * - Scissor rectangles are only used when "rhi::RasterizerState::scissorEnable" is true
 * - The current scissor rectangle(s) does not affect the clear operation
 */
struct SetGraphicsScissorRectangles final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, core::uint32 numberOfScissorRectangles,
                            const ScissorRectangle *scissorRectangles) {
    *commandBuffer.add_command<SetGraphicsScissorRectangles>() = SetGraphicsScissorRectangles(numberOfScissorRectangles,
                                                                                              scissorRectangles);
  }

  static inline void
  create(RHICommandBuffer &commandBuffer, long topLeftX, long topLeftY, long bottomRightX, long bottomRightY) {
    SetGraphicsScissorRectangles *setGraphicsScissorRectanglesCommand = commandBuffer.add_command<SetGraphicsScissorRectangles>(
      sizeof(ScissorRectangle));

    // Set command data
    ScissorRectangle *scissorRectangle = reinterpret_cast<ScissorRectangle *>(CommandPacketHelper::getAuxiliaryMemory(
      setGraphicsScissorRectanglesCommand));
    scissorRectangle->topLeftX = topLeftX;
    scissorRectangle->topLeftY = topLeftY;
    scissorRectangle->bottomRightX = bottomRightX;
    scissorRectangle->bottomRightY = bottomRightY;

    // Finalize command
    setGraphicsScissorRectanglesCommand->numberOfScissorRectangles = 1;
    setGraphicsScissorRectanglesCommand->scissorRectangles = nullptr;
  }

  // Constructor
  inline SetGraphicsScissorRectangles(core::uint32 _numberOfScissorRectangles,
                                      const ScissorRectangle *_scissorRectangles) :
    numberOfScissorRectangles(_numberOfScissorRectangles),
    scissorRectangles(_scissorRectangles) {}

  // Data
  core::uint32 numberOfScissorRectangles;
  const ScissorRectangle *scissorRectangles;  ///< If null pointer, command auxiliary memory is used instead
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_SCISSOR_RECTANGLES;
};

/**
 * @brief
 * Set graphics viewport and scissor rectangle (convenience method)
 *
 * @param[in] topLeftX
 * Top left x
 * @param[in] topLeftY
 * Top left y
 * @param[in] width
 * Width
 * @param[in] height
 * Height
 * @param[in] minimumDepth
 * Minimum depth
 * @param[in] maximumDepth
 * Maximum depth
 *
 * @note
 * - Lookout! In Direct3D 12 the scissor test can't be deactivated and hence one always needs to set a valid scissor rectangle.
 * Use the convenience "rhi::Command::SetGraphicsViewportAndScissorRectangle"-command if possible to not walk into this Direct3D 12 trap.
 */
struct SetGraphicsViewportAndScissorRectangle final {
  // Static methods
  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 topLeftX, core::uint32 topLeftY, core::uint32 width,
         core::uint32 height, float minimumDepth = 0.0f, float maximumDepth = 1.0f) {
    // Set the graphics viewport
    SetGraphicsViewports::create(commandBuffer, topLeftX, topLeftY, width, height, minimumDepth, maximumDepth);

    // Set the graphics scissor rectangle
    SetGraphicsScissorRectangles::create(commandBuffer, static_cast<long>(topLeftX), static_cast<long>(topLeftY),
                                         static_cast<long>(topLeftX + width), static_cast<long>(topLeftY + height));
  }
};

/**
 * @brief
 * Set the graphics render target to render into, output-merger (OM) stage
 *
 * @param[in] renderTarget
 * Render target to render into by binding it to the output-merger state, can be an null pointer to render into the primary window
 */
struct SetGraphicsRenderTarget final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIRenderTarget *renderTarget) {
    *commandBuffer.add_command<SetGraphicsRenderTarget>() = SetGraphicsRenderTarget(renderTarget);
  }

  // Constructor
  inline explicit SetGraphicsRenderTarget(RHIRenderTarget *_renderTarget) :
    renderTarget(_renderTarget) {}

  // Data
  RHIRenderTarget *renderTarget;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_GRAPHICS_RENDER_TARGET;
};

/**
 * @brief
 * Clears the graphics viewport to a specified RGBA color, clears the depth buffer,
 * and erases the stencil buffer
 *
 * @param[in] clearFlags
 * Flags that indicate what should be cleared. This parameter can be any
 * combination of the following flags, but at least one flag must be used:
 * "rhi::ClearFlag::COLOR", "rhi::ClearFlag::DEPTH" and "rhi::ClearFlag::STENCIL, see "rhi::ClearFlag"-flags
 * @param[in] color
 * RGBA clear color (used if "rhi::ClearFlag::COLOR" is set)
 * @param[in] z
 * Z clear value. (if "rhi::ClearFlag::DEPTH" is set)
 * This parameter can be in the range from 0.0 through 1.0. A value of 0.0
 * represents the nearest distance to the viewer, and 1.0 the farthest distance.
 * @param[in] stencil
 * Value to clear the stencil-buffer with. This parameter can be in the range from
 * 0 through 2^n�1, where n is the bit depth of the stencil buffer.
 *
 * @note
 * - The current viewport(s) (see "rhi::Command::set_graphics_viewports()") does not affect the clear operation
 * - The current scissor rectangle(s) (see "rhi::Command::set_graphics_scissor_rectangles()") does not affect the clear operation
 * - In case there are multiple active render targets, all render targets are cleared
 */
struct ClearGraphics final {
  // Static methods
  // -> z = 0 instead of 1 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 clearFlags, const float color[4], float z = 0.0f,
         core::uint32 stencil = 0) {
    *commandBuffer.add_command<ClearGraphics>() = ClearGraphics(clearFlags, color, z, stencil);
  }

  // Constructor
  inline ClearGraphics(core::uint32 _clearFlags, const float _color[4], float _z, core::uint32 _stencil) :
    clearFlags(_clearFlags),
    color{_color[0], _color[1], _color[2], _color[3]},
    z(_z),
    stencil(_stencil) {}

  // Data
  core::uint32 clearFlags;
  float color[4];
  float z;
  core::uint32 stencil;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::CLEAR_GRAPHICS;
};

/**
 * @brief
 * Render the specified geometric primitive, based on an array of vertices instancing and indirect draw
 *
 * @param[in] indirectBuffer
 * Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "rhi::DrawArguments" starting at "indirectBufferOffset"
 * @param[in] indirectBufferOffset
 * Indirect buffer offset
 * @param[in] numberOfDraws
 * Number of draws, can be 0
 *
 * @note
 * - Draw instanced is a shader model 4 feature, only supported if "rhi::Capabilities::drawInstanced" is true
 * - In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
 * "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
 * - Fails if no vertex array is set
 * - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
 * - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
 */
struct DrawGraphics final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, const RHIIndirectBuffer &indirectBuffer,
                            core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1) {
    *commandBuffer.add_command<DrawGraphics>() = DrawGraphics(indirectBuffer, indirectBufferOffset, numberOfDraws);
  }

  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 vertexCountPerInstance, core::uint32 instanceCount = 1,
         core::uint32 startVertexLocation = 0, core::uint32 startInstanceLocation = 0) {
    DrawGraphics *drawCommand = commandBuffer.add_command<DrawGraphics>(sizeof(DrawArguments));

    // Set command data: The command packet auxiliary memory contains an "rhi::DrawArguments"-instance
    const DrawArguments drawArguments(vertexCountPerInstance, instanceCount, startVertexLocation,
                                      startInstanceLocation);
    memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawArguments));

    // Finalize command
    drawCommand->indirectBuffer = nullptr;
    drawCommand->indirectBufferOffset = 0;
    drawCommand->numberOfDraws = 1;
  }

  // Constructor
  inline DrawGraphics(const RHIIndirectBuffer &_indirectBuffer, core::uint32 _indirectBufferOffset,
                      core::uint32 _numberOfDraws) :
    indirectBuffer(&_indirectBuffer),
    indirectBufferOffset(_indirectBufferOffset),
    numberOfDraws(_numberOfDraws) {}

  // Data
  const RHIIndirectBuffer *indirectBuffer;  ///< If null pointer, command auxiliary memory is used instead
  core::uint32 indirectBufferOffset;
  core::uint32 numberOfDraws;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_GRAPHICS;
};

/**
 * @brief
 * Render the specified geometric primitive, based on indexing into an array of vertices, instancing and indirect draw
 *
 * @param[in] indirectBuffer
 * Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "rhi::DrawIndexedArguments" starting at bindirectBufferOffset"
 * @param[in] indirectBufferOffset
 * Indirect buffer offset
 * @param[in] numberOfDraws
 * Number of draws, can be 0
 *
 * @note
 * - Instanced arrays is a shader model 3 feature, only supported if "rhi::Capabilities::instancedArrays" is true
 * - Draw instanced is a shader model 4 feature, only supported if "rhi::Capabilities::drawInstanced" is true
 * - This method draws indexed primitives from the current set of data input streams
 * - Fails if no index and/or vertex array is set
 * - If the multi-draw indirect feature is not supported this parameter, multiple draw calls are emitted
 * - If the draw indirect feature is not supported, a software indirect buffer is used and multiple draw calls are emitted
 */
struct DrawIndexedGraphics final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, const RHIIndirectBuffer &indirectBuffer,
                            core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1) {
    *commandBuffer.add_command<DrawIndexedGraphics>() = DrawIndexedGraphics(indirectBuffer, indirectBufferOffset,
                                                                            numberOfDraws);
  }

  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 indexCountPerInstance, core::uint32 instanceCount = 1,
         core::uint32 startIndexLocation = 0, core::int32 baseVertexLocation = 0,
         core::uint32 startInstanceLocation = 0) {
    DrawIndexedGraphics *drawCommand = commandBuffer.add_command<DrawIndexedGraphics>(sizeof(DrawIndexedArguments));

    // Set command data: The command packet auxiliary memory contains an "rhi::DrawIndexedArguments"-instance
    const DrawIndexedArguments drawIndexedArguments(indexCountPerInstance, instanceCount, startIndexLocation,
                                                    baseVertexLocation, startInstanceLocation);
    memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawIndexedArguments, sizeof(DrawIndexedArguments));

    // Finalize command
    drawCommand->indirectBuffer = nullptr;
    drawCommand->indirectBufferOffset = 0;
    drawCommand->numberOfDraws = 1;
  }

  // Constructor
  inline DrawIndexedGraphics(const RHIIndirectBuffer &_indirectBuffer, core::uint32 _indirectBufferOffset,
                             core::uint32 _numberOfDraws) :
    indirectBuffer(&_indirectBuffer),
    indirectBufferOffset(_indirectBufferOffset),
    numberOfDraws(_numberOfDraws) {}

  // Data
  const RHIIndirectBuffer *indirectBuffer;  ///< If null pointer, command auxiliary memory is used instead
  core::uint32 indirectBufferOffset;
  core::uint32 numberOfDraws;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_INDEXED_GRAPHICS;
};

/**
 * @brief
 * Render the specified geometric primitive, based on a task and mesh shader and indirect draw
 *
 * @param[in] indirectBuffer
 * Indirect buffer to use, the indirect buffer must contain at least "numberOfDraws" instances of "rhi::DrawMeshTasksArguments" starting at "indirectBufferOffset"
 * @param[in] indirectBufferOffset
 * Indirect buffer offset
 * @param[in] numberOfDraws
 * Number of draws, can be 0
 *
 * @note
 * - Only supported if "rhi::Capabilities::meshShader" is "true"
 */
struct DrawMeshTasks final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, const RHIIndirectBuffer &indirectBuffer,
                            core::uint32 indirectBufferOffset = 0, core::uint32 numberOfDraws = 1) {
    *commandBuffer.add_command<DrawMeshTasks>() = DrawMeshTasks(indirectBuffer, indirectBufferOffset, numberOfDraws);
  }

  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 numberOfTasks, core::uint32 firstTask = 0) {
    DrawMeshTasks *drawCommand = commandBuffer.add_command<DrawMeshTasks>(sizeof(DrawMeshTasksArguments));

    // Set command data: The command packet auxiliary memory contains an "rhi::DrawMeshTasksArguments"-instance
    const DrawMeshTasksArguments drawArguments(numberOfTasks, firstTask);
    memcpy(CommandPacketHelper::getAuxiliaryMemory(drawCommand), &drawArguments, sizeof(DrawMeshTasksArguments));

    // Finalize command
    drawCommand->indirectBuffer = nullptr;
    drawCommand->indirectBufferOffset = 0;
    drawCommand->numberOfDraws = 1;
  }

  // Constructor
  inline DrawMeshTasks(const RHIIndirectBuffer &_indirectBuffer, core::uint32 _indirectBufferOffset,
                       core::uint32 _numberOfDraws) :
    indirectBuffer(&_indirectBuffer),
    indirectBufferOffset(_indirectBufferOffset),
    numberOfDraws(_numberOfDraws) {}

  // Data
  const RHIIndirectBuffer *indirectBuffer;  ///< If null pointer, command auxiliary memory is used instead
  core::uint32 indirectBufferOffset;
  core::uint32 numberOfDraws;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DRAW_MESH_TASKS;
};

//[-------------------------------------------------------]
//[ Compute                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Set the used compute root signature
 *
 * @param[in] rootSignature
 * Compute root signature to use, can be an null pointer (default: "nullptr")
 *
 * @note
 * - Only supported if "rhi::Capabilities::computeShader" is true
 */
struct SetComputeRootSignature final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIRootSignature *rootSignature) {
    *commandBuffer.add_command<SetComputeRootSignature>() = SetComputeRootSignature(rootSignature);
  }

  // Constructor
  inline explicit SetComputeRootSignature(RHIRootSignature *_rootSignature) :
    rootSignature(_rootSignature) {}

  // Data
  RHIRootSignature *rootSignature;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_ROOT_SIGNATURE;
};

/**
 * @brief
 * Set the used compute pipeline state
 *
 * @param[in] computePipelineState
 * Compute pipeline state to use, can be an null pointer (default: "nullptr")
 *
 * @note
 * - Only supported if "rhi::Capabilities::computeShader" is true
 */
struct SetComputePipelineState final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIComputePipelineState *computePipelineState) {
    *commandBuffer.add_command<SetComputePipelineState>() = SetComputePipelineState(computePipelineState);
  }

  // Constructor
  inline explicit SetComputePipelineState(RHIComputePipelineState *_computePipelineState) :
    computePipelineState(_computePipelineState) {}

  // Data
  RHIComputePipelineState *computePipelineState;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_PIPELINE_STATE;
};

/**
 * @brief
 * Set a compute resource group
 *
 * @param[in] rootParameterIndex
 * The root parameter index number for binding
 * @param[in] resourceGroup
 * Resource group to set
 *
 * @note
 * - Only supported if "rhi::Capabilities::computeShader" is true
 */
struct SetComputeResourceGroup final {
  // Static methods
  static inline void
  create(RHICommandBuffer &commandBuffer, core::uint32 rootParameterIndex, RHIResourceGroup *resourceGroup) {
    *commandBuffer.add_command<SetComputeResourceGroup>() = SetComputeResourceGroup(rootParameterIndex, resourceGroup);
  }

  // Constructor
  inline SetComputeResourceGroup(core::uint32 _rootParameterIndex, RHIResourceGroup *_resourceGroup) :
    rootParameterIndex(_rootParameterIndex),
    resourceGroup(_resourceGroup) {}

  // Data
  core::uint32 rootParameterIndex;
  RHIResourceGroup *resourceGroup;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_COMPUTE_RESOURCE_GROUP;
};

/**
 * @brief
 * Compute dispatch call
 *
 * @param[in] groupCountX
 * Number of local workgroups to dispatch in the X dimension
 * @param[in] groupCountY
 * Number of local workgroups to dispatch in the Y dimension
 * @param[in] groupCountZ
 * Number of local workgroups to dispatch in the Z dimension
 *
 * @note
 * - Only supported if "rhi::Capabilities::computeShader" is true
 */
struct DispatchCompute final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, core::uint32 groupCountX, core::uint32 groupCountY,
                            core::uint32 groupCountZ) {
    *commandBuffer.add_command<DispatchCompute>() = DispatchCompute(groupCountX, groupCountY, groupCountZ);
  }

  // Constructor
  inline DispatchCompute(core::uint32 _groupCountX, core::uint32 _groupCountY, core::uint32 _groupCountZ) :
    groupCountX(_groupCountX),
    groupCountY(_groupCountY),
    groupCountZ(_groupCountZ) {}

  // Data
  core::uint32 groupCountX;
  core::uint32 groupCountY;
  core::uint32 groupCountZ;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::DISPATCH_COMPUTE;
};

//[-------------------------------------------------------]
//[ Resource                                              ]
//[-------------------------------------------------------]
/**
 * @brief
 * Set texture minimum maximum mipmap index
 *
 * @param[in] texture
 * Texture to set the minimum maximum mipmap index of
 * @param[in] minimumMipmapIndex
 * Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
 * @param[in] maximumMipmapIndex
 * Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
 */
struct SetTextureMinimumMaximumMipmapIndex final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHITexture &texture, core::uint32 minimumMipmapIndex,
                            core::uint32 maximumMipmapIndex) {
    *commandBuffer.add_command<SetTextureMinimumMaximumMipmapIndex>() = SetTextureMinimumMaximumMipmapIndex(texture,
                                                                                                            minimumMipmapIndex,
                                                                                                            maximumMipmapIndex);
  }

  // Constructor
  inline SetTextureMinimumMaximumMipmapIndex(RHITexture &_texture, core::uint32 _minimumMipmapIndex,
                                             core::uint32 _maximumMipmapIndex) :
    texture(&_texture),
    minimumMipmapIndex(_minimumMipmapIndex),
    maximumMipmapIndex(_maximumMipmapIndex) {}

  // Data
  RHITexture *texture;
  core::uint32 minimumMipmapIndex;
  core::uint32 maximumMipmapIndex;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_TEXTURE_MINIMUM_MAXIMUM_MIPMAP_INDEX;
};

/**
 * @brief
 * Resolve multisample framebuffer
 *
 * @param[in] destinationRenderTarget
 * None multisample destination render target
 * @param[in] sourceMultisampleFramebuffer
 * Source multisample framebuffer
 */
struct ResolveMultisampleFramebuffer final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIRenderTarget &destinationRenderTarget,
                            RHIFramebuffer &sourceMultisampleFramebuffer) {
    *commandBuffer.add_command<ResolveMultisampleFramebuffer>() = ResolveMultisampleFramebuffer(destinationRenderTarget,
                                                                                                sourceMultisampleFramebuffer);
  }

  // Constructor
  inline ResolveMultisampleFramebuffer(RHIRenderTarget &_destinationRenderTarget,
                                       RHIFramebuffer &_sourceMultisampleFramebuffer) :
    destinationRenderTarget(&_destinationRenderTarget),
    sourceMultisampleFramebuffer(&_sourceMultisampleFramebuffer) {}

  // Data
  RHIRenderTarget *destinationRenderTarget;
  RHIFramebuffer *sourceMultisampleFramebuffer;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESOLVE_MULTISAMPLE_FRAMEBUFFER;
};

/**
 * @brief
 * Copy resource
 *
 * @param[in] destinationResource
 * Destination resource
 * @param[in] sourceResource
 * Source Resource
 */
struct CopyResource final {
  // Static methods
  static inline void
  create(RHICommandBuffer &commandBuffer, RHIResource &destinationResource, RHIResource &sourceResource) {
    *commandBuffer.add_command<CopyResource>() = CopyResource(destinationResource, sourceResource);
  }

  // Constructor
  inline CopyResource(RHIResource &_destinationResource, RHIResource &_sourceResource) :
    destinationResource(&_destinationResource),
    sourceResource(&_sourceResource) {}

  // Data
  RHIResource *destinationResource;
  RHIResource *sourceResource;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::COPY_RESOURCE;
};

/**
 * @brief
 * Generate resource mipmaps
 *
 * @param[in] resource
 * Resource
 */
struct GenerateMipmaps final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIResource &resource) {
    *commandBuffer.add_command<GenerateMipmaps>() = GenerateMipmaps(resource);
  }

  // Constructor
  inline explicit GenerateMipmaps(RHIResource &_resource) :
    resource(&_resource) {}

  // Data
  RHIResource *resource;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::GENERATE_MIPMAPS;
};

/**
 * @brief
 * Copy uniform buffer data
 *
 * @param[out] uniformBuffer
 * Destination uniform buffer to copy the data to
 * @param[in]  data
 * Data to copy into the destination uniform buffer
 * @param[in]  numberOfBytes
 * Number of bytes to copy
 */
struct CopyUniformBufferData final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIUniformBuffer &uniformBuffer, const void *data,
                            core::uint32 numberOfBytes) {
    CopyUniformBufferData *copyUniformBufferData = commandBuffer.add_command<CopyUniformBufferData>(numberOfBytes);
    copyUniformBufferData->uniformBuffer = &uniformBuffer;
    copyUniformBufferData->numberOfBytes = numberOfBytes;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), data, numberOfBytes);
  }

  // Data
  RHIUniformBuffer *uniformBuffer;
  core::uint32 numberOfBytes;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::COPY_UNIFORM_BUFFER_DATA;
};

/**
 * @brief
 * Set legacy uniform; command is primarily intended for Direct3D 9
 */
struct SetUniform final {
  // Definitions
  enum class Type {
    UNIFORM_1I,
    UNIFORM_1F,
    UNIFORM_2FV,
    UNIFORM_3FV,
    UNIFORM_4FV,
    UNIFORM_MATRIX_3FV,
    UNIFORM_MATRIX_4FV
  };

  // Static methods
  static inline void
  create1i(RHICommandBuffer &commandBuffer, RHIGraphicsProgram &graphicsProgram, core::handle uniformHandle,
           int value) {
    SetUniform *copyUniformBufferData = commandBuffer.add_command<SetUniform>(sizeof(int));
    copyUniformBufferData->graphicsProgram = &graphicsProgram;
    copyUniformBufferData->uniformHandle = uniformHandle;
    copyUniformBufferData->type = Type::UNIFORM_1I;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), &value, sizeof(int));
  }

  static inline void
  create1f(RHICommandBuffer &commandBuffer, RHIGraphicsProgram &graphicsProgram, core::handle uniformHandle,
           float value) {
    SetUniform *copyUniformBufferData = commandBuffer.add_command<SetUniform>(sizeof(float));
    copyUniformBufferData->graphicsProgram = &graphicsProgram;
    copyUniformBufferData->uniformHandle = uniformHandle;
    copyUniformBufferData->type = Type::UNIFORM_1F;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), &value, sizeof(float));
  }

  static inline void
  create2fv(RHICommandBuffer &commandBuffer, RHIGraphicsProgram &graphicsProgram, core::handle uniformHandle,
            const float *value) {
    SetUniform *copyUniformBufferData = commandBuffer.add_command<SetUniform>(sizeof(float) * 2);
    copyUniformBufferData->graphicsProgram = &graphicsProgram;
    copyUniformBufferData->uniformHandle = uniformHandle;
    copyUniformBufferData->type = Type::UNIFORM_2FV;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), value, sizeof(float) * 2);
  }

  static inline void
  create3fv(RHICommandBuffer &commandBuffer, RHIGraphicsProgram &graphicsProgram, core::handle uniformHandle,
            const float *value) {
    SetUniform *copyUniformBufferData = commandBuffer.add_command<SetUniform>(sizeof(float) * 3);
    copyUniformBufferData->graphicsProgram = &graphicsProgram;
    copyUniformBufferData->uniformHandle = uniformHandle;
    copyUniformBufferData->type = Type::UNIFORM_3FV;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), value, sizeof(float) * 3);
  }

  static inline void
  create4fv(RHICommandBuffer &commandBuffer, RHIGraphicsProgram &graphicsProgram, core::handle uniformHandle,
            const float *value) {
    SetUniform *copyUniformBufferData = commandBuffer.add_command<SetUniform>(sizeof(float) * 4);
    copyUniformBufferData->graphicsProgram = &graphicsProgram;
    copyUniformBufferData->uniformHandle = uniformHandle;
    copyUniformBufferData->type = Type::UNIFORM_4FV;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), value, sizeof(float) * 4);
  }

  static inline void
  createMatrix3fv(RHICommandBuffer &commandBuffer, RHIGraphicsProgram &graphicsProgram, core::handle uniformHandle,
                  const float *value) {
    SetUniform *copyUniformBufferData = commandBuffer.add_command<SetUniform>(sizeof(float) * 3 * 3);
    copyUniformBufferData->graphicsProgram = &graphicsProgram;
    copyUniformBufferData->uniformHandle = uniformHandle;
    copyUniformBufferData->type = Type::UNIFORM_MATRIX_3FV;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), value, sizeof(float) * 3 * 3);
  }

  static inline void
  createMatrix4fv(RHICommandBuffer &commandBuffer, RHIGraphicsProgram &graphicsProgram, core::handle uniformHandle,
                  const float *value) {
    SetUniform *copyUniformBufferData = commandBuffer.add_command<SetUniform>(sizeof(float) * 4 * 4);
    copyUniformBufferData->graphicsProgram = &graphicsProgram;
    copyUniformBufferData->uniformHandle = uniformHandle;
    copyUniformBufferData->type = Type::UNIFORM_MATRIX_4FV;
    memcpy(CommandPacketHelper::getAuxiliaryMemory(copyUniformBufferData), value, sizeof(float) * 4 * 4);
  }

  // Data
  RHIGraphicsProgram *graphicsProgram;
  core::handle uniformHandle;
  Type type;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_UNIFORM;
};

//[-------------------------------------------------------]
//[ Query                                                 ]
//[-------------------------------------------------------]
/**
 * @brief
 * reset asynchronous query pool
 *
 * @param[in] queryPool
 * Query pool
 * @param[in] firstQueryIndex
 * First query index (e.g. 0)
 * @param[in] numberOfQueries
 * Number of queries (e.g. 1)
 */
struct ResetQueryPool final {
  // Static methods
  static inline void
  create(RHICommandBuffer &commandBuffer, RHIQueryPool &queryPool, core::uint32 firstQueryIndex = 0,
         core::uint32 numberOfQueries = 1) {
    *commandBuffer.add_command<ResetQueryPool>() = ResetQueryPool(queryPool, firstQueryIndex, numberOfQueries);
  }

  // Constructor
  inline ResetQueryPool(RHIQueryPool &_queryPool, core::uint32 _firstQueryIndex, core::uint32 _numberOfQueries) :
    queryPool(&_queryPool),
    firstQueryIndex(_firstQueryIndex),
    numberOfQueries(_numberOfQueries) {}

  // Data
  RHIQueryPool *queryPool;
  core::uint32 firstQueryIndex;
  core::uint32 numberOfQueries;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::RESET_QUERY_POOL;
};

/**
 * @brief
 * Begin asynchronous query
 *
 * @param[in] queryPool
 * Query pool
 * @param[in] queryIndex
 * Query index (e.g. 0)
 * @param[in] queryControlFlags
 * Query control flags (e.g. "rhi::QueryControlFlags::PRECISE")
 */
struct BeginQuery final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIQueryPool &queryPool, core::uint32 queryIndex = 0,
                            core::uint32 queryControlFlags = 0) {
    *commandBuffer.add_command<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
  }

  // Constructor
  inline BeginQuery(RHIQueryPool &_queryPool, core::uint32 _queryIndex, core::uint32 _queryControlFlags) :
    queryPool(&_queryPool),
    queryIndex(_queryIndex),
    queryControlFlags(_queryControlFlags) {}

  // Data
  RHIQueryPool *queryPool;
  core::uint32 queryIndex;
  core::uint32 queryControlFlags;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_QUERY;
};

/**
 * @brief
 * reset and begin asynchronous query; ease-of-use function in case there's just a single query inside the query pool
 *
 * @param[in] queryPool
 * Query pool
 * @param[in] queryIndex
 * Query index (e.g. 0)
 * @param[in] queryControlFlags
 * Query control flags (e.g. "rhi::QueryControlFlags::PRECISE")
 */
struct ResetAndBeginQuery final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIQueryPool &queryPool, core::uint32 queryIndex = 0,
                            core::uint32 queryControlFlags = 0) {
    *commandBuffer.add_command<ResetQueryPool>() = ResetQueryPool(queryPool, queryIndex, 1);
    *commandBuffer.add_command<BeginQuery>() = BeginQuery(queryPool, queryIndex, queryControlFlags);
  }
};

/**
 * @brief
 * End asynchronous query
 *
 * @param[in] queryPool
 * Query pool
 * @param[in] queryIndex
 * Query index (e.g. 0)
 */
struct EndQuery final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIQueryPool &queryPool, core::uint32 queryIndex = 0) {
    *commandBuffer.add_command<EndQuery>() = EndQuery(queryPool, queryIndex);
  }

  // Constructor
  inline EndQuery(RHIQueryPool &_queryPool, core::uint32 _queryIndex) :
    queryPool(&_queryPool),
    queryIndex(_queryIndex) {}

  // Data
  RHIQueryPool *queryPool;
  core::uint32 queryIndex;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_QUERY;
};

/**
 * @brief
 * Write asynchronous timestamp query
 *
 * @param[in] queryPool
 * Query pool
 * @param[in] queryIndex
 * Query index (e.g. 0)
 */
struct WriteTimestampQuery final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, RHIQueryPool &queryPool, core::uint32 queryIndex = 0) {
    *commandBuffer.add_command<WriteTimestampQuery>() = WriteTimestampQuery(queryPool, queryIndex);
  }

  // Constructor
  inline WriteTimestampQuery(RHIQueryPool &_queryPool, core::uint32 _queryIndex) :
    queryPool(&_queryPool),
    queryIndex(_queryIndex) {}

  // Data
  RHIQueryPool *queryPool;
  core::uint32 queryIndex;
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::WRITE_TIMESTAMP_QUERY;
};

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
/**
 * @brief
 * Set a debug marker
 *
 * @param[in] name
 * ASCII name of the debug marker, must be valid (there's no internal null pointer test)
 *
 * @see
 * - "is_debug_enabled()"
 */
struct SetDebugMarker final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, const char *name) {
    *commandBuffer.add_command<SetDebugMarker>() = SetDebugMarker(name);
  }

  // Constructor
  inline explicit SetDebugMarker(const char *_name) {
    BE_ASSERT(strlen(_name) < 128, "Invalid name")
    strncpy(name, _name, 128);
    name[127] = '\0';
  }

  // Data
  char name[128];
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::SET_DEBUG_MARKER;
};

/**
 * @brief
 * Begin debug event
 *
 * @param[in] name
 * ASCII name of the debug event, must be valid (there's no internal null pointer test)
 *
 * @see
 * - "is_debug_enabled()"
 */
struct BeginDebugEvent final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer, const char *name) {
    *commandBuffer.add_command<BeginDebugEvent>() = BeginDebugEvent(name);
  }

  // Constructor
  inline explicit BeginDebugEvent(const char *_name) {
    BE_ASSERT(strlen(_name) < 128, "Invalid name")
    strncpy(name, _name, 128);
    name[127] = '\0';
  }

  // Data
  char name[128];
  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::BEGIN_DEBUG_EVENT;
};

/**
 * @brief
 * End the last started debug event
 *
 * @see
 * - "is_debug_enabled()"
 */
struct EndDebugEvent final {
  // Static methods
  static inline void create(RHICommandBuffer &commandBuffer) {
    *commandBuffer.add_command<EndDebugEvent>() = EndDebugEvent();
  }

  // Static data
  static constexpr CommandDispatchFunctionIndex COMMAND_DISPATCH_FUNCTION_INDEX = CommandDispatchFunctionIndex::END_DEBUG_EVENT;
};

}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
