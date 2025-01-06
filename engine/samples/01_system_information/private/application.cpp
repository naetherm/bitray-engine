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
#include <core/platform/platform.h>
#include <cstdio>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Application::Application() {

}

Application::~Application() {

}

void Application::main() {
  printf("System Information:\n");

  core::Platform& platform = core::Platform::instance();
  printf("\tPlatform:   %s\n", platform.get_platform().c_str());
  printf("\tOS:         %s\n", platform.get_os().c_str());
  printf("\tCPU Mhz:    %u Mhz\n", platform.get_cpu_mhz());
  printf("\tNum Cores:  %u Cores\n", platform.get_number_cpu_cores());
  printf("\tCur. Dir:   %s\n", platform.get_current_dir().c_str());
  printf("\tExecutable: %s\n", platform.get_executable_filename().c_str());
}
