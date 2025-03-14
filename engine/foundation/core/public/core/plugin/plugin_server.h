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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/core/server_impl.h"
#include "core/container/vector.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * PluginServer
 *
 * @brief
 * PluginServer is an abstract interface for managing plugins.
 *
 * It provides methods for loading, unloading and getting plugins.
 *
 * @tparam TPlugin
 * @tparam TCore
 */
template<typename TPlugin, typename TCore>
class PluginServer : public ServerImpl {
public:

  /**
   * @brief
   * The default constructor for the PluginServer class.
   */
  PluginServer(const String& loaderMethod, TCore* core);

  /**
   * @brief
   * Destructor for the PluginServer class.
   *
   * This will unload all plugins loaded by this server.
   */
  ~PluginServer() override;


  /**
   * @brief Get the list of plugins.
   *
   * @return A constant reference to the vector of plugins.
   */
  const Vector<TPlugin*>& get_plugins() const;

  /**
   * @brief
   * Adds a plugin to the list of plugins.
   *
   * This will not load the plugin. It will only add the plugin to the list. To load the plugin,
   * call the load_plugin() method.
   *
   * @param plugin The plugin to add.
   */
  void add_plugin(TPlugin& plugin);

  /**
   * @brief
   * Load a plugin from a shared library.
   *
   * If the plugin is already loaded, then this method will return false.
   *
   * @param pluginName The name of the plugin.
   * @param filename The filename of the shared library containing the plugin.
   *
   * @return If the plugin was successfully loaded.
   */
  bool load_plugin(const String& pluginName, const String& filename);

  /**
   * @brief
   * Unload all plugins loaded by this server.
   *
   * If you want to unload a single plugin, you should use the unload_plugin() method of the
   * Plugin class.
   */
  void unload_all_plugins();

protected:

  /**
   * @brief
   * Create and load a plugin from the specified shared library.
   *
   * This method attempts to load a plugin from the given shared library file. If successful,
   * the plugin is instantiated, initialized, and added to the list of managed plugins.
   *
   * @param pluginName The name of the plugin to create.
   * @param filename The filename of the shared library containing the plugin.
   *
   * @return A pointer to the created plugin if successful, nullptr otherwise.
   */
  TPlugin* create_plugin(const String& pluginName, const String& filename);

protected:
  String mLoaderMethod;
  TCore* mCore;
  /** List of all loaded plugins */
  Vector<TPlugin*> mPlugins;
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


#include "core/plugin/plugin_server.inl"