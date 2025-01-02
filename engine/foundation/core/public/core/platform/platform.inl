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
#include "core/platform/platform_impl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
inline Version Platform::get_version() {
  static Version SVersion = Version(RE_NAME, "RE", RE_VERSION_MAJOR, RE_VERSION_MINOR, RE_VERSION_PATCH, RE_VERSION_RELEASE);
  return SVersion;
}
String Platform::get_platform() const {
  return this->mImpl->get_platform();
}

String Platform::get_os() const {
  return this->mImpl->get_os();
}

char Platform::get_separator() const {
  return this->mImpl->get_separator();
}

String Platform::get_shared_library_prefix() const {
  return this->mImpl->get_shared_library_prefix();
}

String Platform::get_shared_library_extension() const {
  return this->mImpl->get_shared_library_extension();
}

uint32 Platform::get_cpu_mhz() const {
  return mImpl->get_cpu_mhz();
}

CPUInfo Platform::get_cpu_info() const {
  return mImpl->get_cpu_info();
}

uint32 Platform::get_number_cpu_cores() const {
  return mImpl->get_cpu_info().LogicalProcessorCount;
}

String Platform::get_executable_filename() const {
  return this->mImpl->get_executable_filename();
}

String Platform::get_current_dir() const {
  return mImpl->get_current_dir();
}

void Platform::urgent_message(const String &sMessage) const {
  this->mImpl->urgent_message(sMessage);
}

uint64 Platform::get_milliseconds() const {
  return mImpl->get_milliseconds();
}

uint64 Platform::get_microseconds() const {
  return mImpl->get_microseconds();
}

void Platform::sleep(uint64 nMilliseconds) const {
  this->mImpl->sleep(nMilliseconds);
}

void Platform::yield() const {
  mImpl->yield();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
