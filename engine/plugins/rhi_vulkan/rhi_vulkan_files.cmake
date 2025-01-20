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
  # private
  private/mapping.cpp
  private/rhidynamicrhi.cpp
  private/rhiresource_group.cpp
  private/rhiroot_signature.cpp
  private/shader_compiler.cpp
  private/vulkan_context.cpp
  private/vulkan_helper.cpp
  private/vulkan_runtime_linking.cpp

  # private/buffer
  private/buffer/rhibuffer_manager.cpp
  private/buffer/rhiindex_buffer.cpp
  private/buffer/rhiindirect_buffer.cpp
  private/buffer/rhistructured_buffer.cpp
  private/buffer/rhitexture_buffer.cpp
  private/buffer/rhiuniform_buffer.cpp
  private/buffer/rhivertex_array.cpp
  private/buffer/rhivertex_buffer.cpp

  # private/query
  private/query/rhiquery_pool.cpp

  # private/render_target
  private/render_target/rhiframebuffer.cpp
  private/render_target/rhirender_pass.cpp
  private/render_target/rhiswap_chain.cpp

  # private/shader
  private/shader/rhicompute_shader_glsl.cpp
  private/shader/rhifragment_shader_glsl.cpp
  private/shader/rhigeometry_shader_glsl.cpp
  private/shader/rhigraphics_program_glsl.cpp
  private/shader/rhimesh_shader_glsl.cpp
  private/shader/rhishader_language_glsl.cpp
  private/shader/rhitask_shader_glsl.cpp
  private/shader/rhitessellation_control_shader_glsl.cpp
  private/shader/rhitessellation_evaluation_shader_glsl.cpp
  private/shader/rhivertex_shader_glsl.cpp

  # private/state
  private/state/rhicompute_pipeline_state.cpp
  private/state/rhigraphics_pipeline_state.cpp
  private/state/rhisampler_state.cpp

  # private/texture
  private/texture/rhitexture_1d.cpp
  private/texture/rhitexture_1d_array.cpp
  private/texture/rhitexture_2d.cpp
  private/texture/rhitexture_2d_array.cpp
  private/texture/rhitexture_3d.cpp
  private/texture/rhitexture_cube.cpp
  private/texture/rhitexture_cube_array.cpp
  private/texture/rhitexture_manager.cpp
)