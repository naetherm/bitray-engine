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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "rhi/rhi.h"
#include "rhi/rhidynamicrhi.h"
#include "rhi/rhiresource_group.h"
#include "rhi/rhiroot_signature.h"
#include "rhi/rhicapabilities.h"
#include "rhi/rhicontext.h"
#include "rhi/rhiresource.h"
#include "rhi/rhistatistics.h"
#include "rhi/rhicommand_buffer.h"
#include "rhi/buffer/rhibuffer.h"
#include "rhi/buffer/rhistructured_buffer.h"
#include "rhi/buffer/rhibuffer_manager.h"
#include "rhi/buffer/rhiindex_buffer.h"
#include "rhi/buffer/rhiuniform_buffer.h"
#include "rhi/buffer/rhitexture_buffer.h"
#include "rhi/buffer/rhiindirect_buffer.h"
#include "rhi/buffer/rhivertex_array.h"
#include "rhi/buffer/rhivertex_buffer.h"
#include "rhi/render_target/rhirender_target.h"
#include "rhi/render_target/rhiswap_chain.h"
#include "rhi/render_target/rhiframebuffer.h"
#include "rhi/render_target/rhirender_pass.h"
#include "rhi/query/rhiquery_pool.h"
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