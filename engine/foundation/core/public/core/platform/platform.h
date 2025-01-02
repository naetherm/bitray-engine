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
#include "core/platform/cpu_info.h"
#include "core/string/string.h"
#include "core/tools/version.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class PlatformImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class CORE_API Platform {
public:
  /**
   * @brief
   * Static singleton instance getter.
   *
   * @return
   * Reference to singleton instance.
   */
  static Platform& instance();

protected:
  /**
   * @brief
   * Constructor.
   */
  Platform();

/**
 * @brief
 * Destructor.
 */
  ~Platform();

public:

  [[nodiscard]] static inline Version get_version();

public:

  /**
   * @brief
   * Returns the name of the platform
   *
   * @return
   * Platform string (for instance 'Windows' for Windows, 'Linux' for Linux and so on)
   */
  [[nodiscard]] inline String get_platform() const;

  /**
   * @brief
   * Returns the name and version of the operating system
   *
   * @return
   * OS information string (for instance 'Windows 7 Service Pack 1 (Build 7601)')
   */
  [[nodiscard]] inline String get_os() const;

  /**
   * @brief
   * Returns the directory separator used by the operation system
   *
   * @return
   * The directory separator used by the operation system (e.g. '/' on Linux, '\' on Windows)
   */
  [[nodiscard]] inline char get_separator() const;

  /**
   * @brief
   * Returns the shared library filename prefix used by the operation system
   *
   * @return
   * The shared library filename prefix used by the operation system (e.g. 'lib' as in 'libcore.so' on Linux, no prefix as in 'core.dll' on Windows)
   */
  [[nodiscard]] inline String get_shared_library_prefix() const;

  /**
   * @brief
   * Returns the shared library file extension used by the operation system
   *
   * @return
   * The shared library file extension used by the operation system (e.g. 'so' on Linux, 'dll' on Windows)
   */
  [[nodiscard]] inline String get_shared_library_extension() const;

  /**
   * @brief
   * Gets the CPU Mhz.
   *
   * @return
   * CPU Mhz.
   */
  [[nodiscard]] inline uint32 get_cpu_mhz() const;

  /**
   * @brief
   * Returns the general cpu information.
   *
   * @return
   * General cpu information.
   */
  [[nodiscard]] inline CPUInfo get_cpu_info() const;

  /**
   * @brief
   * Returns the number of cpu cores available on the current machine.
   *
   * @return
   * Number of available cpu cores.
   */
  [[nodiscard]] inline uint32 get_number_cpu_cores() const;

  /**
   * @brief
   * get absolute path of application executable
   *
   * @return
   * Path to executable (native path style, e.g. on Windows: 'C:\MyApplication\Test.exe')
   *
   * @note
   * - Application executable = currently running process
   */
  [[nodiscard]] inline String get_executable_filename() const;

  /**
   * @brief
   * Returns the current directory
   *
   * @return
   * Path to the current directory as the OS provided it (native path style)
   *
   * @note
   * - On Windows for example "C:\Programs\App"
   */
  [[nodiscard]] inline String get_current_dir() const;

  /**
   * @brief
   * Primitive way (e.g. by using a message box) to be able to tell the user that something went terrible wrong
   *
   * @param[in] sMessage
   * Message to show
   *
   * @remarks
   * Do not misuse this method in order to communicate with the user on a regular basis. This method does only
   * exist to be able to tell the user that something went terrible wrong. There are situations were one can't
   * use a log file, command line or something like this. Even when using e.g. a log file to write out error
   * information - an application may e.g. just close directly after it's start without any further information
   * and the user may even think that the application didn't start in the first place for an unknown reason.
   * In such a situation, it's polite to inform the user that something went terrible wrong and providing
   * a short hint how the issue may be solved. This method wasn't named "MessageBox()" by intend - because
   * such a feature may not be available on the used platform or is handled in another way as a normal MS Windows
   * message box.
   */
  inline void urgent_message(const String &sMessage) const;


  //[-------------------------------------------------------]
  //[ Time                                                  ]
  //[-------------------------------------------------------]

  /**
   * @brief
   * Returns the number of milliseconds since the system was started.
   *
   * @return
   * The number of milliseconds since the system was started.
   */
  inline uint64 get_milliseconds() const;

  /**
   * @brief
   * Returns the number of microseconds since the system was started.
   *
   * @return
   * The number of microseconds since the system was started.
   */
  inline uint64 get_microseconds() const;

  /**
   * @brief
   * Suspend the current thread for a specified time period
   *
   * @param[in] nMilliseconds
   * Number of milliseconds to sleep, should not be 0 because the behavior is implementation dependent (use 'Yield()' instead)
   */
  inline void sleep(uint64 nMilliseconds) const;

  /**
   * @brief
   * Yields the rest of the current threads time slice.
   */
  inline void yield() const;

private:

  PlatformImpl* mImpl;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/platform/platform.inl"