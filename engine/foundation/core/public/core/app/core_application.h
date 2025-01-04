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
#include "core/core/abstract_lifecycle.h"
#include "core/container/vector.h"
#include "core/string/string.h"
#include "core/tools/command_line.h"
#include "core/tools/version.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * CoreApplication
 *
 * @brief
 * Core application implementation is just a bare minimum implementation skeleton for an application.
 * This is usually used for creating minimal CLI tools.
 */
class CoreApplication : public AbstractLifecycle {

public:
  /**
   * @brief
   * Global run method, which is executed in every re_main method for starting the application.
   *
   * @return
   * The return exit code.
   */
  int run(const String& executableName, const Vector<String>& arguments);

public:
  /**
   * @brief
   * Constructor.
   */
  CoreApplication();

  /**
   * @brief
   * Destructor.
   */
  ~CoreApplication() override;

 public:

  /**
   * @brief
   * Returns the command line interface.
   *
   * @return
   * Command line interface.
   */
  [[nodiscard]] inline const CommandLine& get_command_line() const;

  //[-------------------------------------------------------]
  //[ Protected virtual CoreApplication functions           ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Main function
   *
   * @remarks
   * The default implementation does the following tasks:
   * - none (implement in derived classes)
   */
  CORE_API virtual void main();

  /**
   * @brief
   * Called when application should initialize itself
   *
   * @remarks
   * The default implementation does the following tasks:
   * - reset timing class
   * - Return and go on with main()
   */
  CORE_API virtual void on_init();

  /**
   * @brief
   * Called when application should initialize its logging.
   */
  CORE_API virtual bool on_init_log();

  /**
   * @brief
   * Called when application should check command line options
   *
   * @remarks
   * The default implementation does the following tasks:
   * - Check for command line errors or the parameters --help or --version
   * - On '--help' or on error, call OnPrintHelp() and exit
   * - On '--version', call on_print_version() and exit
   *
   * @note
   * - Part of the application framework initialization function "on_start()"
   * - To end the application here, use Application::exit()
   */
  CORE_API virtual void on_init_cmd_line();

  /**
   * @brief
   * Called when application should initialize its configuration.
   */
  CORE_API virtual bool on_init_config();

  /**
   * @brief
   * Function that is called to display a help message about the application
   *
   * @remarks
   * The default implementation prints the application title and it's command line
   * options onto the screen
   */
  CORE_API virtual void on_print_help();

  /**
   * @brief
   * Function that is called to display version information of the application
   *
   * @remarks
   * The default implementation prints the application title and version that is
   * stored in Application
   */
  CORE_API virtual void on_print_version();

  /**
   * @brief
   * Called when application should de-initialize itself
   *
   * @remarks
   * The default implementation does the following tasks:
   * - none (implement in derived classes)
   * - Return and go on with de_init()
   */
  CORE_API virtual void on_de_init();

  //[-------------------------------------------------------]
  //[ Public virtual AbstractLifecycle functions            ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Called directly after the object has been created
   */
  void on_create() override;

  /**
   * @brief
   * Called directly before a stopped object is going to start again (always followed by "on_start()")
   */
  void on_restart() override;

  /**
   * @brief
   * Called when the object becoming visible to the user
   *
   * @return
   * 'true' if all went fine, else 'false' (on failure, no "on_resume()", "on_pause()" or "on_stop()" will be called)
   */
  [[nodiscard]] bool on_start() override;

  /**
   * @brief
   * Called when the object has the focus (keep the implementation lightweight)
   */
  void on_resume() override;

  /**
   * @brief
   * Called when the object has no longer the focus (keep the implementation lightweight)
   */
  void on_pause() override;

  /**
   * @brief
   * Called when the object is no longer visible to the user
   */
  void on_stop() override;

  /**
   * @brief
   * Called before the object is going to be finally destroyed
   */
  void on_destroy() override;

protected:
  /** The command line interface */
  CommandLine mCommandLine;
  /** The version of the application */
  Version mVersion;
  /** The exit code of the application */
  int mExitCode;
  /** Flag for determining whether to exit */
  bool mExit;
  /** Flag for determining whether we are still running */
  bool mRunning;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/app/core_application.inl"