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
#include "core/core.h"
#include "core/core/assert_impl.h"

#include <cstring>
#include <mutex>

#include <iostream>
#include <cstdarg>



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * DefaultAssert
 *
 * @brief
 * Default assert implementation class one can use
 *
 * @note
 * - Example: RE_ASSERT(mContext, is_initialized, "Direct3D 11 RHI implementation assert failed")
 * - Designed to be instanced and used inside a single C++ file
 * - On Microsoft Windows it will print to the Visual Studio output console and the debugger will break
 * - On Linux it will print on the console
 */
class DefaultAssert : public AssertImpl {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
  /**
   * @brief
   * Default constructor.
   */
  DefaultAssert() = default;

  /**
   * @brief
   * Destructor
   */
  ~DefaultAssert() override = default;


//[-------------------------------------------------------]
//[ Public virtual core::IAssert methods                   ]
//[-------------------------------------------------------]
public:
  [[nodiscard]] bool handle_assert(const char* expression, const char* file, uint32 line, const char* format, ...);


//[-------------------------------------------------------]
//[ Protected virtual Rhi::DefaultAssert methods          ]
//[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Receives an already formatted message for further processing
   *
   * @param[in] expression
   * Expression as ASCII string
   * @param[in] file
   * File as ASCII string
   * @param[in] line
   * Line number
   * @param[in] message
   * UTF-8 message
   * @param[in] numberOfCharacters
   * Number of characters inside the message, does not include the terminating zero character
   *
   * @return
   * "true" to request debug break, else "false"
   */
  [[nodiscard]] static bool handle_assert_internal(const char* expression, const char* file, uint32 line, const char* message, uint32 numberOfCharacters);


//[-------------------------------------------------------]
//[ Protected data                                        ]
//[-------------------------------------------------------]
protected:
  //static core::Mutex mMutex;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
  explicit DefaultAssert(const DefaultAssert&) = delete;
  DefaultAssert& operator=(const DefaultAssert&) = delete;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


// Macros & definitions
/**
 * @brief
 * Ease-of-use assert macro, potential issues checked by asserts shouldn't happen in shipped builds and hence are optimized out there
 *
 * @param[in] expression
 * Expression which must be true, else the assert triggers
 * @param[in] format
 * "snprintf"-style formatted UTF-8 assert message
 *
 * @note
 * - Example: RE_ASSERT(is_initialized, "Direct3D 11 RHI implementation assert failed")
 * - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
 */
#ifdef DEBUG
#define RE_ASSERT(expression, format, ...) \
			do { \
				if (!(expression) && core::DefaultAssert().handle_assert(#expression, __FILE__, static_cast<core::uint32>(__LINE__), format, ##__VA_ARGS__)) { \
					DEBUG_BREAK; \
				} \
			} while (0);
#define RE_ASSERT_ONCE(expression, format, ...) \
			do { \
				static bool loggedOnce = false; \
				if (!loggedOnce && !(expression)) { \
					loggedOnce = true; \
					if (core::DefaultAssert().handle_assert(#expression, __FILE__, static_cast<core::uint32>(__LINE__), format, ##__VA_ARGS__)) { \
						DEBUG_BREAK; \
					} \
				} \
			} while (0);
#else
#define RE_ASSERT(expression, format, ...)
#define RE_ASSERT_ONCE(expression, format, ...)
#endif