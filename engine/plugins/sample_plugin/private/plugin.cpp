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
#include "sample_plugin/plugin.h"
#include <core/log/log.h>
#include <engine/engine/engine_core.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace sample_plugin {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class SampleEnginePlugin : public engine::EnginePlugin {
public:

  SampleEnginePlugin(engine::EngineCore* core) {

  }

  ~SampleEnginePlugin() override {

  }

  bool on_install() override {
    BE_LOG(Info, "SampleEnginePlugin::on_install")
    return true;
  }

  bool on_startup() override {
    BE_LOG(Info, "SampleEnginePlugin::on_startup")
    return true;
  }

  void on_shutdown() override {
    BE_LOG(Info, "SampleEnginePlugin::on_shutdown")
  }

  void on_uninstall() override {
    BE_LOG(Info, "SampleEnginePlugin::on_uninstall")
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}

extern "C" engine::EnginePlugin* load_engine_plugin(engine::EngineCore* engine) {
  BE_LOG(Info, "load_engine_plugin()")
  return new sample_plugin::SampleEnginePlugin(engine);
}
