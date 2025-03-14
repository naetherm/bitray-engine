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
#include "core/platform/library.h"
#include "core/plugin/plugin.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TPlugin, typename TCore>
PluginServer<TPlugin, TCore>::PluginServer(const String& loaderMethod, TCore* core)
: mLoaderMethod(loaderMethod)
, mCore(core) {
}

template<typename TPlugin, typename TCore>
PluginServer<TPlugin, TCore>::~PluginServer() {
  unload_all_plugins();
}

template<typename TPlugin, typename TCore>
const Vector<TPlugin*>& PluginServer<TPlugin, TCore>::get_plugins() const {
  return mPlugins;
}

template<typename TPlugin, typename TCore>
void PluginServer<TPlugin, TCore>::add_plugin(TPlugin& plugin) {
  if (std::find(mPlugins.begin(), mPlugins.end(), &plugin) == mPlugins.end()) {
    mPlugins.push_back(&plugin);
  }
}

template<typename TPlugin, typename TCore>
bool PluginServer<TPlugin, TCore>::load_plugin(const String& pluginName, const String& filename) {
  // Check if we can find this plugin in the current list
  for (auto* plugin : mPlugins) {
    if (plugin->get_filename() == filename) return false;
  }

  TPlugin* plugin = create_plugin(pluginName, filename);
  if (plugin != nullptr) {
    return true;
  }

  // Error
  return false;
}

template<typename TPlugin, typename TCore>
void PluginServer<TPlugin, TCore>::unload_all_plugins() {
  for (auto* plugin : mPlugins) {
    plugin->on_shutdown();

    plugin->on_uninstall();

    // Close plugin library
    Library* library = plugin->get_library();
    delete plugin;
    if (library->unload()) {
      re_delete(library);
    }
    library = nullptr;
    plugin = nullptr;
  }

  mPlugins.clear();
}


template<typename TPlugin, typename TCore>
TPlugin* PluginServer<TPlugin, TCore>::create_plugin(const core::String& pluginName, const core::String& filename) {
  // Load the plugin
  Library* library = re_new<Library>();
  library->set_path(Path(filename));
  if (library->load()) {
    // Get the correct plugin loading symbol
    // Construct the correct plugin name
    typedef TPlugin* (*LOAD_ENGINE_PLUGIN)(TCore*);
    auto pluginLoader = reinterpret_cast<LOAD_ENGINE_PLUGIN>(library->get_symbol(mLoaderMethod));
    if (pluginLoader) {
      TPlugin* plugin = pluginLoader(mCore);
      plugin->set_filename(filename);
      plugin->set_library(library);
      plugin->on_install();
      plugin->on_startup();

      mPlugins.push_back(plugin);

      return plugin;
    }
  }
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}