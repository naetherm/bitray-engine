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
#include <cstdio>
#include <core/platform/platform.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Application::Application()
: mEngineCore(re_new<engine::EngineCore>())
, mEnginePluginServer(mEngineCore) {

}

Application::~Application() {

}

void Application::main() {
  printf("Hello Plugin :)\n");

  core::String pluginPath = core::Platform::instance().get_shared_library_prefix() + "sample_plugin." + core::Platform::instance().get_shared_library_extension();
  mEnginePluginServer.load_plugin("sample_plugin", pluginPath);

}

void Application::on_destroy() {
  mEnginePluginServer.unload_all_plugins();

  re_delete(mEngineCore);

  CoreApplication::on_destroy();
}
