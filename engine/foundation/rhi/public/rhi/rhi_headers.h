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
#include "rhi/rhidynamicrhi.h"
#include "rhi/rhidynamicrhi_types.h"
#include "rhi/rhiresource_group.h"
#include "rhi/rhiroot_signature.h"
#include "rhi/rhiroot_signature_types.h"
#include "rhi/rhicapabilities.h"
#include "rhi/rhicontext.h"
#include "rhi/rhiresource.h"
#include "rhi/rhistatistics.h"
#include "rhi/rhicommand_buffer.h"
#include "rhi/buffer/rhibuffer.h"
#include "rhi/buffer/rhibuffer_types.h"
#include "rhi/buffer/rhistructured_buffer.h"
#include "rhi/buffer/rhibuffer_manager.h"
#include "rhi/buffer/rhiindex_buffer.h"
#include "rhi/buffer/rhiindex_buffer_types.h"
#include "rhi/buffer/rhiuniform_buffer.h"
#include "rhi/buffer/rhitexture_buffer.h"
#include "rhi/buffer/rhiindirect_buffer.h"
#include "rhi/buffer/rhiindirect_buffer_types.h"
#include "rhi/buffer/rhivertex_array.h"
#include "rhi/buffer/rhivertex_array_types.h"
#include "rhi/buffer/rhivertex_buffer.h"
#include "rhi/render_target/rhirender_target.h"
#include "rhi/render_target/rhiswap_chain.h"
#include "rhi/render_target/rhiframebuffer.h"
#include "rhi/render_target/rhirender_pass.h"
#include "rhi/query/rhiquery_pool.h"
#include "rhi/query/rhiquery_types.h"
#include "rhi/shader/rhicompute_shader.h"
#include "rhi/shader/rhimesh_shader.h"
#include "rhi/shader/rhitask_shader.h"
#include "rhi/shader/rhifragment_shader.h"
#include "rhi/shader/rhitessellation_control_shader.h"
#include "rhi/shader/rhitessellation_evaluation_shader.h"
#include "rhi/shader/rhivertex_shader.h"
#include "rhi/shader/rhigeometry_shader.h"
#include "rhi/shader/rhishader_language.h"
#include "rhi/shader/rhigraphics_program.h"
#include "rhi/shader/rhishader.h"
#include "rhi/shader/rhishader_types.h"
#include "rhi/state/rhigraphics_pipeline_state.h"
#include "rhi/state/rhicompute_pipeline_state.h"
#include "rhi/state/rhisampler_state.h"
#include "rhi/state/rhipipeline_state.h"
#include "rhi/state/rhistate.h"
#include "rhi/texture/rhitexture_cube_array.h"
#include "rhi/texture/rhitexture_cube.h"
#include "rhi/texture/rhitexture_2d_array.h"
#include "rhi/texture/rhitexture_2d.h"
#include "rhi/texture/rhitexture_1d_array.h"
#include "rhi/texture/rhitexture_1d.h"
#include "rhi/texture/rhitexture_3d.h"
#include "rhi/texture/rhitexture_manager.h"
#include "rhi/texture/rhitexture.h"


namespace rhi {

	// Debug macros
	#ifdef DEBUG
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(command_buffer, name) rhi::Command::SetDebugMarker::create(command_buffer, name);

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(command_buffer) rhi::Command::SetDebugMarker::create(command_buffer, __FUNCTION__);

		/**
		*  @brief
		*    Begin debug event
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(command_buffer, name) rhi::Command::BeginDebugEvent::create(command_buffer, name);

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(command_buffer) rhi::Command::BeginDebugEvent::create(command_buffer, __FUNCTION__);

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(command_buffer) rhi::Command::EndDebugEvent::create(command_buffer);

		/**
		*  @brief
		*    Scoped debug event class
		*/
		class CommandScopedDebugEventOnExit
		{
		// Public methods
		public:
			inline explicit CommandScopedDebugEventOnExit(rhi::RHICommandBuffer& command_buffer) :
        mCommandBuffer(command_buffer)
			{}

			inline ~CommandScopedDebugEventOnExit()
			{
				rhi::Command::EndDebugEvent::create(mCommandBuffer);
			}

		// Private methods
		private:
			explicit CommandScopedDebugEventOnExit(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;
			CommandScopedDebugEventOnExit& operator =(const CommandScopedDebugEventOnExit& commandScopedDebugEventOnExit) = delete;

		// Private data
		private:
      rhi::RHICommandBuffer& mCommandBuffer;
		};

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(command_buffer, name) \
			rhi::Command::BeginDebugEvent::create(command_buffer, name); \
			PRAGMA_WARNING_PUSH \
				PRAGMA_WARNING_DISABLE_MSVC(4456) \
				rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(command_buffer); \
			PRAGMA_WARNING_POP

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(command_buffer) \
			rhi::Command::BeginDebugEvent::create(command_buffer, __FUNCTION__); \
			PRAGMA_WARNING_PUSH \
				PRAGMA_WARNING_DISABLE_MSVC(4456) \
				rhi::CommandScopedDebugEventOnExit commandScopedDebugEventOnExit##__FUNCTION__(command_buffer); \
			PRAGMA_WARNING_POP
	#else
		/**
		*  @brief
		*    Set a debug marker
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*  @param[in] name
		*    ASCII name of the debug marker
		*/
		#define COMMAND_SET_DEBUG_MARKER(command_buffer, name)

		/**
		*  @brief
		*    Set a debug marker by using the current function name ("__FUNCTION__") as marker name
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_SET_DEBUG_MARKER_FUNCTION(command_buffer)

		/**
		*  @brief
		*    Begin debug event, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT(command_buffer, name)

		/**
		*  @brief
		*    Begin debug event by using the current function name ("__FUNCTION__") as event name, must be ended by using "COMMAND_END_DEBUG_EVENT()"
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(command_buffer)

		/**
		*  @brief
		*    End the last started debug event
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_END_DEBUG_EVENT(command_buffer)

		/**
		*  @brief
		*    Scoped debug event, minor internal overhead compared to manual begin/end
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*  @param[in] name
		*    ASCII name of the debug event
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT(command_buffer, name)

		/**
		*  @brief
		*    Scoped debug event by using the current function name ("__FUNCTION__") as event name, minor internal overhead compared to manual begin/end
		*
		*  @param[in] command_buffer
		*    Reference to the rhi instance to use
		*/
		#define COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(command_buffer)
	#endif




//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Rhi



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