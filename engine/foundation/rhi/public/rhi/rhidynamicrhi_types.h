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

#if defined(LINUX)

#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace rhi {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IRenderWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Holds information about an window into which the rendering should be done
 *
 * @note
 * - One of those members must be valid
 */
struct WindowHandle final {
  core::handle nativeWindowHandle;  /// The native window handle
  IRenderWindow *renderWindow;    /// A pointer to an "rhi::IRenderWindow"-instance, can be a null pointer
#ifdef LINUX
  // TODO(naetherm): wl_surface*	waylandSurface;		/// A Wayland surface can't be put into a handle type, so we store a pointer to the Wayland surface here
#else
  void*		unused;				/// For writing e.g. "rhi::WindowHandle{nativeWindowHandle, nullptr, nullptr}" for all platforms // TODO(naetherm) We might want to implement another solution like "WindowHandle::fromNativeWindowHandle()", "WindowHandle::fromRenderWindow()", "WindowHandle::fromWaylandSurface()", while there we could add a type and then using a data-union
#endif
};

/**
 * @brief
 * Map types
 *
 * @note
 * - These constants directly map to Direct3D 10 & 11 constants, do not change them
 */
enum class MapType {
  READ = 1,
  WRITE = 2,
  READ_WRITE = 3,
  WRITE_DISCARD = 4,
  WRITE_NO_OVERWRITE = 5
};

/**
 * @brief
 * Map flags
 *
 * @note
 * - These constants directly map to Direct3D 11 constants, do not change them
 */
struct MapFlag final {
  enum Enum {
    /// In case the resource is currently used when "rhi::Irhi::map()" is called, let the method return with an error, cannot be used with "rhi::MapType::WRITE_DISCARD" or "rhi::MapType::WRITE_NO_OVERWRITE"
    DO_NOT_WAIT = 0x100000L  
  };
};

/**
 * @brief
 * Clear flags
 */
struct ClearFlag final {
  enum Enum {
    /// Clear color buffer
    COLOR = 1 << 0,
    /// Clear depth buffer
    DEPTH = 1 << 1,
    /// Clear stencil buffer
    STENCIL = 1 << 2,
    /// Clear color and depth buffer
    COLOR_DEPTH = COLOR | DEPTH  
  };
};

/**
 * @brief
 * Comparison function
 *
 * @note
 * - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb204902%28v=vs.85%29.aspx are used in here
 * - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
 *
 * @see
 * - "D3D12_COMPARISON_FUNC"-documentation for details
 */
enum class ComparisonFunc {
  /// Never pass the comparison
  NEVER = 1,
  /// If the source data is less than the destination data, the comparison passes
  LESS = 2,
  /// If the source data is equal to the destination data, the comparison passes
  EQUAL = 3,
  /// If the source data is less than or equal to the destination data, the comparison passes
  LESS_EQUAL = 4,
  /// If the source data is greater than the destination data, the comparison passes
  GREATER = 5,
  /// If the source data is not equal to the destination data, the comparison passes
  NOT_EQUAL = 6,
  /// If the source data is greater than or equal to the destination data, the comparison passes
  GREATER_EQUAL = 7,
  /// Always pass the comparison
  ALWAYS = 8  
};

/**
 * @brief
 * Color write enable flags
 *
 * @note
 * - These constants directly map to Direct3D 10 & 11 constants, do not change them
 */
struct ColorWriteEnableFlag final {
  enum Enum {
    RED = 1,
    GREEN = 2,
    BLUE = 4,
    ALPHA = 8,
    ALL = (((RED | GREEN) | BLUE) | ALPHA)
  };
};

/**
 * @brief
 * Mapped subresource
 *
 * @note
 * - This structure directly maps to Direct3D 11, do not change it
 */
struct MappedSubresource final {
  /**
   * @brief
   * Pointer to the data of the mapped subresource.
   *
   * This pointer allows direct access to the subresource's memory for read or write operations.
   */
  void *data;

  /**
   * @brief
   * The row pitch, in bytes, of the subresource.
   *
   * Row pitch specifies the number of bytes between the beginning of one row of a texture and the
   * beginning of the next row. Useful for navigating 2D textures in memory.
   */
  core::uint32 rowPitch;

  /**
   * @brief
   * The depth pitch, in bytes, of the subresource.
   *
   * Depth pitch specifies the number of bytes between the beginning of one depth slice of a 3D
   * texture and the beginning of the next depth slice. Useful for navigating 3D textures in memory.
   */
  core::uint32 depthPitch;
};

/**
 * @brief
 * Viewport
 *
 * @note
 * - This structure directly maps to Direct3D 11 & 12 as well as Vulkan, do not change it
 *
 * @see
 * - "D3D12_VIEWPORT" or "VkViewport" documentation for details
 */
struct Viewport final {
  /// Top left x start position
  float topLeftX;
  /// Top left y start position
  float topLeftY;
  /// Viewport width
  float width;
  /// Viewport height
  float height;
  /// Minimum depth value, usually 0.0f, between [0, 1]
  float minDepth;
  /// Maximum depth value, usually 1.0f, between [0, 1]
  float maxDepth;  
};

/**
 * @brief
 * Scissor rectangle
 *
 * @note
 * - This structure directly maps to Direct3D 9 & 10 & 11 & 12, do not change it
 *
 * @see
 * - "D3D12_RECT"-documentation for details
 */
struct ScissorRectangle final {
  /// Top left x-coordinate of the scissor rectangle
  long topLeftX;
  /// Top left y-coordinate of the scissor rectangle
  long topLeftY;
  /// Bottom right x-coordinate of the scissor rectangle
  long bottomRightX;
  /// Bottom right y-coordinate of the scissor rectangle
  long bottomRightY;  
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // rhi
