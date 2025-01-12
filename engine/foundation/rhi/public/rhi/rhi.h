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


#include <core/core.h>
#include <core/core/assert_impl.h>
#include "rhi/descriptor/descriptors.h"


#ifdef DEBUG
/**
 * @brief
 * Resource name for debugging purposes, ignored when not using "DEBUG"
 *
 * @param[in] debugName
 * ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
 */
#define RHI_RESOURCE_DEBUG_NAME_PARAMETER , const char debugName[] = ""
#define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT , const char debugName[]

/**
 * @brief
 * Pass resource name for debugging purposes, ignored when not using "DEBUG"
 */
#define RHI_RESOURCE_DEBUG_PASS_PARAMETER , debugName
#else
/**
 * @brief
 * Resource name for debugging purposes, ignored when not using "DEBUG"
 *
 * @param[in] debugName
 * ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
 */
#define RHI_RESOURCE_DEBUG_NAME_PARAMETER
#define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT

/**
 * @brief
 * Pass resource name for debugging purposes, ignored when not using "DEBUG"
 */
#define RHI_RESOURCE_DEBUG_PASS_PARAMETER
#endif




//[-------------------------------------------------------]
//[ Debug macros                                          ]
//[-------------------------------------------------------]
#ifdef DEBUG
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name) if (nullptr != rhi) { (rhi)->setDebugMarker(name); }

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->setDebugMarker(__FUNCTION__); }

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name) if (nullptr != rhi) { (rhi)->beginDebugEvent(name); }

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi) if (nullptr != rhi) { (rhi)->beginDebugEvent(__FUNCTION__); }

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_END_DEBUG_EVENT(rhi) if (nullptr != rhi) { (rhi)->endDebugEvent(); }

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name) , name

	/**
	*  @brief
	*    Decorate the debug name to make it easier to see the semantic of the resource
	*
	*  @param[in] name
	*    Debug name provided from the outside
	*  @param[in] decoration
	*    Decoration to append in front (e.g. "IBO", will result in appended "IBO: " in front if the provided name isn't empty)
	*  @param[in] numberOfDecorationCharacters
	*    Number of decoration characters
	*
	*  @note
	*    - The result is in local string variable "detailedName"
	*    - Traditional C-string on the runtime stack used for efficiency reasons (just for debugging, but must still be some kind of usable)
	*/
	#define RHI_DECORATED_DEBUG_NAME(name, detailedName, decoration, numberOfDecorationCharacters) \
		BE_LOG_CONDITION(strlen(name) < 256, Error, "Name is not allowed to exceed 255 characters") \
		char detailedName[256 + numberOfDecorationCharacters] = decoration; \
		if (name[0] != '\0') \
		{ \
			strcat(detailedName, ": "); \
			strncat(detailedName, name, 256); \
		}
#else
	/**
	*  @brief
	*    Set a debug marker
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug marker
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_SET_DEBUG_MARKER(rhi, name)

	/**
	*  @brief
	*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_SET_DEBUG_MARKER_FUNCTION(rhi)

	/**
	*  @brief
	*    Begin debug event
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*  @param[in] name
	*    ASCII name of the debug event
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_BEGIN_DEBUG_EVENT(rhi, name)

	/**
	*  @brief
	*    Begin debug event by using the current function name ("__FUNCTION__") as event name
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_BEGIN_DEBUG_EVENT_FUNCTION(rhi)

	/**
	*  @brief
	*    End the last started debug event
	*
	*  @param[in] rhi
	*    Pointer to the rhi instance to use, can be a null pointer
	*
	*  @note
	*    - Only for rhi implementation internal usage, don't expose it inside the public rhi header
	*/
	#define RHI_END_DEBUG_EVENT(rhi)

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "DEBUG"
	*
	*  @param[in] name
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME(name)
#endif