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
#include "gui/gui.h"
#include <core/color/color4.h>
#include <core/math/vec2.h>
#include <imgui.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace gui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * ImGuiHelper
 *
 * @brief
 * Helper structure for "POD"-Type conversion.
 */
class ImGuiHelper {
public:

  /** Constructor */
  ImGuiHelper() = delete;

  /** Destructor */
  ~ImGuiHelper() = delete;

public:

  /**
   * @brief
   * Convert Vec2i to ImGui ImVec2.
   */
  static ImVec2 to_imvec2(const core::Vec2i& v);

  /**
   * @brief
   * Convert ImGui ImVec2 to Vec2i.
   */
  static core::Vec2i to_vec2i(const ImVec2& v);

  /**
   * @brief
   * Convert Color4 to ImGui ImVec4.
   */
  static ImVec4 to_imvec4(const core::Color4& c);

  /**
   * @brief
   * Convert ImGui ImVec4 to Color4.
   */
  static core::Color4 to_color4(const ImVec4& c);
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // gui