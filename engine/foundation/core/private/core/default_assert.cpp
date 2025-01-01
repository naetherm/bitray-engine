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
#include "core/core/default_assert.h"

#if defined(WIN32)

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
  // Set Windows version to Windows Vista (0x0600), we don't support Windows XP (0x0501)
  #ifdef WINVER
    #undef WINVER
  #endif
  #define WINVER			0x0600
  #ifdef _WIN32_WINNT
    #undef _WIN32_WINNT
  #endif
  #define _WIN32_WINNT	0x0600

  // Exclude some stuff from "windows.h" to speed up compilation a bit
  #define WIN32_LEAN_AND_MEAN
  #define NOGDICAPMASKS
  #define NOMENUS
  #define NOICONS
  #define NOKEYSTATES
  #define NOSYSCOMMANDS
  #define NORASTEROPS
  #define OEMRESOURCE
  #define NOATOM
  #define NOMEMMGR
  #define NOMETAFILE
  #define NOOPENFILE
  #define NOSCROLL
  #define NOSERVICE
  #define NOSOUND
  #define NOWH
  #define NOCOMM
  #define NOKANJI
  #define NOHELP
  #define NOPROFILER
  #define NODEFERWINDOWPOS
  #define NOMCX
  #define NOCRYPT
  #include <Windows.h>

  // get rid of some nasty OS macros
  #undef min
  #undef max
PRAGMA_WARNING_POP
#elif defined(ANDROID)
#include <android/log.h>
#elif defined(LINUX)
#include <iostream>
#include <cstdarg>
#else
#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


bool DefaultAssert::handle_assert(const char* expression, const char* file, uint32 line, const char* format, ...) {
  bool requestDebugBreak = false;

  // get the required buffer length, does not include the terminating zero character
  va_list vaList;
  va_start(vaList, format);
  const uint32 textLength = static_cast<uint32>(vsnprintf(nullptr, 0, format, vaList));
  va_end(vaList);
  if (256 > textLength) {
    // Fast path: C-runtime stack

    // Construct the formatted text
    char formattedText[256];	// 255 +1 for the terminating zero character
    va_start(vaList, format);
    vsnprintf(formattedText, 256, format, vaList);
    va_end(vaList);

    // Internal processing
    requestDebugBreak = DefaultAssert::handle_assert_internal(expression, file, line, formattedText, textLength);
  } else {
    // Slow path: Heap
    // -> No reused scratch buffer in order to reduce memory allocation/deallocations in here to not make things more complex and to reduce the mutex locked scope

    // Construct the formatted text
    char* formattedText = new char[textLength + 1];	// 1+ for the terminating zero character
    va_start(vaList, format);
    vsnprintf(formattedText, textLength + 1, format, vaList);
    va_end(vaList);

    // Internal processing
    requestDebugBreak = DefaultAssert::handle_assert_internal(expression, file, line, formattedText, textLength);

    // Cleanup
    delete [] formattedText;
  }

  // Done
  return requestDebugBreak;
}

[[nodiscard]] bool DefaultAssert::handle_assert_internal(const char* expression, const char* file, uint32 line, const char* message, uint32) {
  //std::lock_guard<std::mutex> mutexLock(mMutex);
  bool requestDebugBreak = false;

  // Construct the full UTF-8 message text
  std::string fullMessage = "Assert message in " + std::string(file) + ":" + std::to_string(line) + ": " + std::string(message) + "\" | Expression \"" + std::string(expression) + "\"";
  if ('\n' != fullMessage.back()) {
    fullMessage += '\n';
  }

  // Platform specific handling
#if defined(WIN32)
  {
    // Convert UTF-8 string to UTF-16
    std::wstring utf16Line;
    utf16Line.resize(static_cast<std::wstring::size_type>(::MultiByteToWideChar(CP_UTF8, 0, fullMessage.c_str(), static_cast<int>(fullMessage.size()), nullptr , 0)));
    ::MultiByteToWideChar(CP_UTF8, 0, fullMessage.c_str(), static_cast<int>(fullMessage.size()), utf16Line.data(), static_cast<int>(utf16Line.size()));

    // Write into standard output stream
    std::wcerr << utf16Line.c_str();

    // On Microsoft Windows, ensure the output can be seen inside the Visual Studio output window as well
    ::OutputDebugStringW(utf16Line.c_str());
    if (::IsDebuggerPresent())
    {
      requestDebugBreak = true;
    }
  }
#elif defined(ANDROID)
  __android_log_write(ANDROID_LOG_DEBUG, RE_NAME, fullMessage.c_str());
  requestDebugBreak = true;
#elif defined(LINUX)
  // Write into standard output stream
  std::cerr << fullMessage.c_str();
  requestDebugBreak = true;
#else
#error "Unsupported platform"
#endif

  // Done
  return requestDebugBreak;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core