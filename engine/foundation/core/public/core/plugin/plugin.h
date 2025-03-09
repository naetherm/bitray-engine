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
#include "core/core/non_copyable.h"
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
 * Plugin
 *
 * @brief
 * A base class for plugins, ensuring plugins are non-copyable.
 *
 * The Plugin class provides a foundation for creating plugins in a
 * software application, inheriting from NonCopyable to prevent copying.
 */
class Plugin : public NonCopyable {

  friend class PluginServer;

public:

  /**
   * @brief
   * Default constructor.
   */
  Plugin();

  /**
   * @brief
   * Destructor.
   */
  ~Plugin() override;


  /**
   * @brief Get the filename of the plugin.
   *
   * @return The filename as a constant reference to a String.
   */
  const String& get_filename() const;

public:

  /**
   * @brief
   * Function called when the plugin is installed.
   *
   * @return true if the plugin was successfully installed, false otherwise.
   */
  virtual bool on_install() = 0;

  /**
   * @brief
   * Function called when the plugin is started up.
   *
   * @return true if the plugin was successfully started, false otherwise.
   */
  virtual bool on_startup() = 0;

  /**
   * @brief
   * Function called when the plugin is shut down.
   */
  virtual void on_shutdown() = 0;

  /**
   * @brief
   * Function called when the plugin is uninstalled.
   */
  virtual void on_uninstall() = 0;

protected:
  /** The filename of the plugin */
  String mFileName;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}