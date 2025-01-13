#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2025 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////


set(FILES
  private/rhiroot_signature.cpp
  private/rhiresource_group.cpp
  private/extensions.cpp
  private/helper.cpp
  private/mapping.cpp
  private/opengl_runtime_linking.cpp
  private/rhidynamicrhi.cpp
  private/rhiopengl_context.cpp
  private/shader_compiler.cpp
  # buffer
  private/buffer/rhivertex_buffer.cpp
  private/buffer/rhivertex_array.cpp
  private/buffer/rhiuniform_buffer.cpp
  private/buffer/rhitexture_buffer.cpp
  private/buffer/rhistructured_buffer.cpp
  private/buffer/rhiindirect_buffer.cpp
  private/buffer/rhiindex_buffer.cpp
  private/buffer/rhibuffer_manager.cpp
  private/buffer/rhiindex_buffer_bind.cpp
  private/buffer/rhiindex_buffer_dsa.cpp
  private/buffer/rhiindirect_buffer_bind.cpp
  private/buffer/rhiindirect_buffer_dsa.cpp
  private/buffer/rhistructured_buffer_bind.cpp
  private/buffer/rhistructured_buffer_dsa.cpp
  private/buffer/rhitexture_buffer_bind.cpp
  private/buffer/rhitexture_buffer_dsa.cpp
  private/buffer/rhiuniform_buffer_bind.cpp
  private/buffer/rhiuniform_buffer_dsa.cpp
  private/buffer/rhivertex_array_no_vao.cpp
  private/buffer/rhivertex_array_vao.cpp
  private/buffer/rhivertex_array_vao_bind.cpp
  private/buffer/rhivertex_array_vao_dsa.cpp
  private/buffer/rhivertex_buffer_bind.cpp
  private/buffer/rhivertex_buffer_dsa.cpp
  # query
  private/query/rhiquery_pool.cpp
  private/query/rhipipeline_statistics_query_pool.cpp
  private/query/rhiocclusion_timestamp_query_pool.cpp
  # render_target
  private/render_target/rhiswap_chain.cpp
  private/render_target/rhirender_pass.cpp
  private/render_target/rhiframebuffer.cpp
  private/render_target/rhiframebuffer_bind.cpp
  private/render_target/rhiframebuffer_dsa.cpp
  # shader
  # shader/monolithic
  private/shader/monolithic/rhicompute_shader_monolithic.cpp
  private/shader/monolithic/rhifragment_shader_monolithic.cpp
  private/shader/monolithic/rhigeometry_shader_monolithic.cpp
  private/shader/monolithic/rhigraphics_program_monolithic.cpp
  private/shader/monolithic/rhigraphics_program_monolithic_dsa.cpp
  private/shader/monolithic/rhimesh_shader_monolithic.cpp
  private/shader/monolithic/rhishader_language_monolithic.cpp
  private/shader/monolithic/rhitask_shader_monolithic.cpp
  private/shader/monolithic/rhitessellation_control_shader_monolithic.cpp
  private/shader/monolithic/rhitessellation_evaluation_shader_monolithic.cpp
  private/shader/monolithic/rhivertex_shader_monolithic.cpp
  # shader/separate
  private/shader/separate/rhicompute_shader_separate.cpp
  private/shader/separate/rhifragment_shader_separate.cpp
  private/shader/separate/rhigeometry_shader_separate.cpp
  private/shader/separate/rhigraphics_program_separate.cpp
  private/shader/separate/rhigraphics_program_separate_dsa.cpp
  private/shader/separate/rhimesh_shader_separate.cpp
  private/shader/separate/rhishader_language_separate.cpp
  private/shader/separate/rhitask_shader_separate.cpp
  private/shader/separate/rhitessellation_control_shader_separate.cpp
  private/shader/separate/rhitessellation_evaluation_shader_separate.cpp
  private/shader/separate/rhivertex_shader_separate.cpp
  # state
  private/state/rhisampler_state.cpp
  private/state/rhigraphics_pipeline_state.cpp
  private/state/rhicompute_pipeline_state.cpp
  private/state/rhiblend_state.cpp
  private/state/rhicompute_pipeline_state_monolithic.cpp
  private/state/rhicompute_pipeline_state_separate.cpp
  private/state/rhidepth_stencil_state.cpp
  private/state/rhiopengl_sampler_state.cpp
  private/state/rhirasterizer_state.cpp
  private/state/rhisampler_state_bind.cpp
  private/state/rhisampler_state_dsa.cpp
  private/state/rhisampler_state_so.cpp
  # texture
  private/texture/rhitexture_cube_array.cpp
  private/texture/rhitexture_manager.cpp
  private/texture/rhitexture_cube.cpp
  private/texture/rhitexture_cube_array.cpp
  private/texture/rhitexture_3d.cpp
  private/texture/rhitexture_2d_array.cpp
  private/texture/rhitexture_2d.cpp
  private/texture/rhitexture_1d_array.cpp
  private/texture/rhitexture_1d.cpp
  private/texture/rhitexture_1d_array_bind.cpp
  private/texture/rhitexture_1d_array_dsa.cpp
  private/texture/rhitexture_1d_bind.cpp
  private/texture/rhitexture_1d_dsa.cpp
  private/texture/rhitexture_2d_array_bind.cpp
  private/texture/rhitexture_2d_array_dsa.cpp
  private/texture/rhitexture_2d_bind.cpp
  private/texture/rhitexture_2d_dsa.cpp
  private/texture/rhitexture_3d_bind.cpp
  private/texture/rhitexture_3d_dsa.cpp
  private/texture/rhitexture_cube_array_bind.cpp
  private/texture/rhitexture_cube_array_dsa.cpp
  private/texture/rhitexture_cube_bind.cpp
  private/texture/rhitexture_cube_dsa.cpp
)