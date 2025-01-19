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
#include "application.h"
#include <core/frontend/window_device.h>
#include <core/platform/platform.h>
#include <core/log/log.h>
#include <rhi/rhi_headers.h>

#include "empty/empty.h"
#include "compute_shader/compute_shader.h"
#include "cube_texture/cube_texture.h"
#include "geometry_shader/geometry_shader.h"
#include "indirect_buffer/indirect_buffer.h"
#include "instancing/instancing.h"
#include "mesh_shader/mesh_shader.h"
#include "queries/queries.h"
#include "render_to_texture/render_to_texture.h"
#include "tessellation_shader/tessellation_shader.h"
#include "texture/texture.h"
#include "triangle/triangle.h"
#include "vertex_buffer/vertex_buffer.h"
#include "gpgpu/gpgpu.h"
#include "icosahedron_tessellation/icosahedron_tessellation.h"
#include "instanced_cubes/instanced_cubes.h"


Application::Application()
: core::FrontendApplication()
, mRhi(nullptr)
, mMainSwapChain(nullptr)
, mExampleBase(nullptr) {
  // Register command line options:
  // [-r/--rhi] The rendering hardware interface
  // [-e/--example] The example to execute
  mCommandLine.add_parameter("RHI", "-r", "--rhi", "Low-Level RHI, one of: OpenGL", "", true);
  mCommandLine.add_parameter("Example", "-e", "--example",
                               "One of: Empty, ComputeShader, CubeTexture, GeometryShader, RHIIndirectBuffer, Instancing, MeshShader, Queries, RenderToTexture, TessellationShader, Texture, Triangle VertexBuffer, Gpgpu, IcosahedronTessellation, InstancedCubes",
                               "", true);
}

Application::~Application() {
  if (this->mExampleBase) {
    delete mExampleBase;
    mExampleBase = nullptr;
  }
  if (this->mMainSwapChain) {
    delete mMainSwapChain;
    mMainSwapChain = nullptr;
  }
  if (this->mRhi) {
    delete mRhi;
    mRhi = nullptr;
  }

  if (this->mpRHIContext) {
    delete mpRHIContext;
    mpRHIContext = nullptr;
  }

  mRhiSharedLibrary.unload();
}


bool Application::on_start() {
  if (core::FrontendApplication::on_start()) {
    auto window = get_window_device()->create_window({"20_low_level_rendering", core::Recti(0, 0, 800, 600), core::EWindowStyle::WindowStyle_NoBorder});

    core::String sRHI = mCommandLine.get_value("RHI");
    core::String sExample = mCommandLine.get_value("Example");

    // Generate library name
    this->msLibraryName = core::Platform::instance().get_shared_library_prefix() + "rhi_" + sRHI + "." +
      core::Platform::instance().get_shared_library_extension();

    this->on_create_renderer_context();

    // Create swap chain
    const rhi::Capabilities& cCapabilities = this->mRhi->get_capabilities();
    rhi::RHIRenderPass* pRenderPass = this->mRhi->create_render_pass(
      1,
      &cCapabilities.preferredSwapChainColorTextureFormat,
      cCapabilities.preferredSwapChainDepthStencilTextureFormat,
      1);
    
    this->mMainSwapChain = this->mRhi->create_swap_chain(*pRenderPass,
        this->get_window_device()->get_window(0)->get_platform_data(), mRhi->get_context().is_using_external_context() RHI_RESOURCE_DEBUG_NAME("Main"));
    this->mMainSwapChain->add_ref();

    // Create and initialize the example
    if (sExample == "Empty") { this->mExampleBase = new Empty(*this, "Empty"); }
    if (sExample == "ComputeShader") { this->mExampleBase = new ComputeShader(*this, "ComputeShader"); }
    if (sExample == "CubeTexture") { this->mExampleBase = new CubeTexture(*this, "CubeTexture"); }
    if (sExample == "GeometryShader") { this->mExampleBase = new GeometryShader(*this, "GeometryShader"); }
    if (sExample == "IndirectBuffer") { this->mExampleBase = new IndirectBuffer(*this, "IndirectBuffer"); }
    if (sExample == "Instancing") { this->mExampleBase = new Instancing(*this, "Instancing"); }
    if (sExample == "MeshShader") { this->mExampleBase = new MeshShader(*this, "MeshShader"); }
    if (sExample == "Queries") { this->mExampleBase = new Queries(*this, "Queries"); }
    if (sExample == "RenderToTexture") { this->mExampleBase = new RenderToTexture(*this, "RenderToTexture"); }
    if (sExample == "TessellationShader") { this->mExampleBase = new TessellationShader(*this, "TessellationShader"); }
    if (sExample == "Texture") { this->mExampleBase = new Texture(*this, "Texture"); }
    if (sExample == "Triangle") { this->mExampleBase = new Triangle(*this, "Triangle"); }
    if (sExample == "VertexBuffer") { this->mExampleBase = new VertexBuffer(*this, "VertexBuffer"); }
    if (sExample == "Gpgpu") { this->mExampleBase = new Gpgpu(*this, "Gpgpu"); }
    if (sExample == "IcosahedronTessellation") { this->mExampleBase = new IcosahedronTessellation(*this, "IcosahedronTessellation"); }
    if (sExample == "InstancedCubes") { this->mExampleBase = new InstancedCubes(*this, "InstancedCubes"); }

    if (this->mExampleBase) {
      this->mExampleBase->on_initialization();
    }

    return true;
  }

  return false;
}

void Application::on_stop() {
  this->mExampleBase->on_deinitialization();

  FrontendApplication::on_stop();
}


void Application::on_size() {
  core::FrontendApplication::on_size();
}

void Application::on_update() {
  FrontendApplication::on_update();

  rhi::Command::SetGraphicsRenderTarget::create(this->mCommandBuffer, this->mMainSwapChain);

  { // Since Direct3D 12 is command list based, the viewport and scissor rectangle must be set in every draw call to work with all supported RHI implementations
    // Get the window size
    uint32_t width  = 1;
    uint32_t height = 1;
    mMainSwapChain->get_width_and_height(width, height);

    // Set the graphics viewport and scissor rectangle
    rhi::Command::SetGraphicsViewportAndScissorRectangle::create(mCommandBuffer, 0, 0, width, height);
  }

  this->mExampleBase->on_update();
}

void Application::on_draw() {
  if (this->mExampleBase) {
    this->mExampleBase->on_draw(mCommandBuffer);
    this->mCommandBuffer.dispatch_to_rhi_and_clear(*this->mRhi);
    this->mMainSwapChain->present();
  } else {
    BE_LOG(Info, "Invalid example")
  }
  FrontendApplication::on_draw();
}



void Application::main() {
  while (true && get_window_device()->get_num_windows() > 0) {
    on_update();

    on_draw();
  }
}

void Application::on_create_renderer_context() {
  // Load and initialize the rhi backend
  // Create the RHI context
  core::FrontendPlatformData platformData;
#if defined(LINUX)
  platformData.os_linux.connection = this->get_window_device()->get_window(0)->get_platform_data().os_linux.connection; // reinterpret_cast<core::LinuxWindowOS *>(reinterpret_cast<core::FrontendOS *>(this->get_frontend().get_impl())->get_window())->get_display();
  platformData.os_linux.window = this->get_window_device()->get_window(0)->get_native_window_handle();
#elif defined(WINDOWS)
#endif
#if defined(LINUX)
  this->mpRHIContext = new rhi::RHIContext(
    platformData
  );
#endif

  // Initialize library
  auto* pLib = new core::Library();
  pLib->set_path(core::Path(this->msLibraryName));
  if (pLib->load()) {
    typedef rhi::RHIDevice* (*RHI_INSTANCER)();
    RHI_INSTANCER _Creator = reinterpret_cast<RHI_INSTANCER>(pLib->get_symbol("create_rhi_device_instance"));
    if (_Creator) {
      this->mRhi = _Creator();
      this->mRhi->initialize(*this->mpRHIContext);

      if (this->mRhi) {
        this->mRhi->add_ref();
        BE_LOG(Info, core::String("Successfully created the RHI backend of ") + this->msLibraryName)
      } else {
        // Error
        BE_LOG(Error, "Creation of dynamic RHI failed.")
      }
    } else {
      // Error
      BE_LOG(Error, "Unable to find the symbol 'create_rhi_device_instance'")
    }
  } else {
    // Error
    BE_LOG(Error, core::String("Unable to load the library ") + this->msLibraryName)
  }

  delete pLib;
}
