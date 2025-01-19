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
  private/main.cpp
  private/application.cpp
  private/example_base.cpp

  private/empty/empty.cpp
  private/compute_shader/compute_shader.cpp
  private/cube_texture/cube_texture.cpp
  private/geometry_shader/geometry_shader.cpp
  private/indirect_buffer/indirect_buffer.cpp
  private/instancing/instancing.cpp
  private/mesh_shader/mesh_shader.cpp
  private/queries/queries.cpp
  private/render_to_texture/render_to_texture.cpp
  private/tessellation_shader/tessellation_shader.cpp
  private/texture/texture.cpp
  private/triangle/triangle.cpp
  private/vertex_buffer/vertex_buffer.cpp

  private/gpgpu/gpgpu.cpp
  private/icosahedron_tessellation/icosahedron_tessellation.cpp
  private/instanced_cubes/instanced_cubes.cpp
  private/instanced_cubes/cube_renderer_draw_instanced/batch_draw_instanced.cpp
  private/instanced_cubes/cube_renderer_draw_instanced/cube_renderer_draw_instanced.cpp
  private/instanced_cubes/cube_renderer_instanced_arrays/batch_instanced_arrays.cpp
  private/instanced_cubes/cube_renderer_instanced_arrays/cube_renderer_instanced_arrays.cpp
)