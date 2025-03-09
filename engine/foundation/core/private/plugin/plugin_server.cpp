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
#include "core/plugin/plugin_server.h"
#include "core/platform/library.h"
#include "core/plugin/plugin.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
PluginServer::PluginServer() {
}

PluginServer::~PluginServer() {
  unload_all_plugins();
}

const Vector<Plugin*>& PluginServer::get_plugins() const {
  return mPlugins;
}

void PluginServer::add_plugin(Plugin& plugin) {
  if (std::find(mPlugins.begin(), mPlugins.end(), &plugin) == mPlugins.end()) {
    mPlugins.push_back(&plugin);
  }
}

bool PluginServer::load_plugin(const String& pluginName, const String& filename) {
  // Check if we can find this plugin in the current list
  for (auto* plugin : mPlugins) {
    if (plugin->get_filename() == filename) return false;
  }

  // Load the plugin
  Library library;
  library.set_path(Path(filename));
  if (library.load()) {
    // Get the correct plugin loading symbol
    // Construct the correct plugin name
    String symbolName = "load_plugin";
    typedef Plugin* (*LOAD_PLUGIN)();
    auto pluginLoader = reinterpret_cast<LOAD_PLUGIN>(library.get_symbol(symbolName));
    if (pluginLoader) {
      Plugin* plugin = pluginLoader();
      plugin->mFileName = filename;
      plugin->on_install();
      plugin->on_startup();

      mPlugins.push_back(plugin);

      return true;
    }
  }

  // Error
  return false;
}

void PluginServer::unload_all_plugins() {
  for (auto* plugin : mPlugins) {
    plugin->on_shutdown();

    plugin->on_uninstall();

    delete plugin;;
  }

  mPlugins.clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}