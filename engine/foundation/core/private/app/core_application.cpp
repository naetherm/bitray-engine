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
#include "core/app/core_application.h"


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
int CoreApplication::run(const String& executableName, const Vector<String>& arguments) {
  // Parse command line
  mCommandLine.parse_command_line(arguments);

  // Do the life cycle thing - let the world know that we have been created (must be done in here instead of within the constructor)
  on_create();

  // The application is now running
  mRunning = true;

  // Initialize application
  mExitCode = 0;

  if (on_start()) {
    on_resume();

    // Call application-specific initialization routine
    on_init();

    // Run application
    main();

    // Call application-specific de-initialization routine
    on_de_init();

    // Do the frontend life cycle thing - de-initialize
    on_pause();
    on_stop();
  }

  // The application is no longer running
  mRunning = false;

  // Do the life cycle thing - let the world know that we're going to die (should be done in here instead of within the destructor)
  on_destroy();

  // Exit
  return mExitCode;
}

CoreApplication::CoreApplication()
: mExitCode(0)
, mExit(false)
, mRunning(false) {

  mCommandLine.add_flag("Help", "-h", "--help", "Display help");
  mCommandLine.add_flag("About", "-v", "--version", "Display version");
}

CoreApplication::~CoreApplication() {

}


void CoreApplication::main() {

}

void CoreApplication::on_init() {

}

bool CoreApplication::on_init_log() {
  return true;
}

void CoreApplication::on_init_cmd_line() {
  // Check command line
  if (mCommandLine.has_errors() || mCommandLine.is_value_set("--help")) {
    // Display help and exit
    on_print_help();
    exit(1);
  } else if (mCommandLine.is_value_set("--version")) {
    // Display about and exit
    on_print_version();
    exit(0);
  }
}

bool CoreApplication::on_init_config() {
  return true;
}

void CoreApplication::on_print_help() {
  // Print application title
  //Platform::instance().get_console().print(mCoreApplicationContext.get_executable_filename() + "\n\n");

  // Print available command line options
  //mCommandLine.print_help(mCoreApplicationContext.get_executable_filename().substr(mCoreApplicationContext.get_executable_filename().rfind("/") + 1));
}

void CoreApplication::on_print_version() {
  // Print application title and version
  //Platform::instance().get_console().print("- V" + Platform::get_version().to_string() + '\n');
}

void CoreApplication::on_de_init() {

}

void CoreApplication::on_create() {

}

void CoreApplication::on_restart() {

}

bool CoreApplication::on_start() {
  mRunning = true;
  // Initialize log system
  if (!on_init_log()) {
    mRunning = false;

    return mRunning;
  }
  // Check command line
  on_init_cmd_line();

  if (!mRunning)
    return false;

  if (!on_init_config()) {
    mRunning = false;

    return mRunning;
  }

  return mRunning;
}

void CoreApplication::on_resume() {

}

void CoreApplication::on_pause() {

}

void CoreApplication::on_stop() {

}

void CoreApplication::on_destroy() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}