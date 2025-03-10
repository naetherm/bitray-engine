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
#ifdef ANDROID
#include <android/log.h>
#else

#include <stdio.h>  // For "fputs" and "stdout" inside "Log::Write()"

#endif

#include "core/log/log_formatter_text.h"
#include "core/log/log.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
// For fast Log::LogLevelToString()
const String g_sLogLevelToString[] = {
  "Quiet",
  "All",
  "Critical",
  "Error",
  "Warning",
  "Info",
  "Debug"
};


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
Log &Log::instance() {
  // The compiler should be able to optimize this extra call, at least inside this project (inlining)
  static Log SInstance;
  return SInstance;
}



//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
void Log::set_buffer_count(uint32 nBufferCount) {
  // Make an explicit flush
  flush();

  // Remove surplus log messages
  if (m_nBufferedMessages > nBufferCount) {
    const uint32 nToRemove = m_nBufferedMessages - nBufferCount;
    for (uint32 i = 0; i < nToRemove; i++)
      m_qLastMessages.pop();
  }

  // Set the new number of buffered messaged
  m_nBufferedMessages = nBufferCount;
}

String Log::log_level_to_string(uint8 nLogLevel) const {
  if (!nLogLevel)
    return "";
  else if (nLogLevel > Debug)
    return String("Debug") + String(nLogLevel - Debug);
  else
    return g_sLogLevelToString[nLogLevel - 1];
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
Log::Log() :
  m_bVerbose(true),
  m_bVerboseLogLevelPrefix(true),
  m_nLogLevel(Info),
  m_nFlushLogLevel(Error),
  m_nFlushMessages(0),
  m_nBufferedMessages(1000),
  m_pLogFormatter(nullptr) {
}

Log::Log(const Log &cSource) :
  m_bVerbose(true),
  m_bVerboseLogLevelPrefix(true),
  m_nLogLevel(Info),
  m_nFlushLogLevel(Error),
  m_nFlushMessages(0),
  m_nBufferedMessages(1000),
  m_pLogFormatter(nullptr) {
  // No implementation because the copy constructor is never used
}

Log::~Log() {
  // Close log
}

Log &Log::operator=(const Log &cSource) {
  // No implementation because the copy operator is never used
  return *this;
}

bool Log::write(uint8 nLogLevel, const String &sText) {
  // Is there any text to write down?
  if (sText.length()) {
    // Write into the standard OS console?
    if (m_bVerbose) {
      // Prepare log message
      String sLogMessage;
      if (nLogLevel >= Quiet && m_bVerboseLogLevelPrefix) {
        sLogMessage = '[';
        sLogMessage += log_level_to_string(nLogLevel);
        sLogMessage += "]: ";
      }
      sLogMessage += sText;
      sLogMessage += '\n';

      // Write the text into the OS console - it would be nice if this could be done using 'File::StandardOutput.Print()',
      // but this may cause problems when de-initializing the static variables :/
#ifdef WIN32
      (sLogMessage.GetFormat() == String::ASCII) ? fputs(sLogMessage.cstr(), stdout) : fputws(sLogMessage.GetUnicode(), stdout);
#elif ANDROID
      // Lookout! "__android_log_write" doesn't check for null pointer!
      const char *pszLogMessage = sLogMessage.cstr();
      if (pszLogMessage) {
        android_LogPriority nAndroidLogPriority = ANDROID_LOG_DEFAULT;
        switch (nLogLevel) {
          // No log outputs
          case Quiet:
            nAndroidLogPriority = ANDROID_LOG_SILENT;
            break;

          // Should be used if the message should be logged always
          case Always:
            nAndroidLogPriority = ANDROID_LOG_VERBOSE;
            break;

          // Should be used if an critical error occurs
          case Critical:
            nAndroidLogPriority = ANDROID_LOG_FATAL;
            break;

          // Should be used if an error occurs
          case Error:
            nAndroidLogPriority = ANDROID_LOG_ERROR;
            break;

          // Should be used for warning texts
          case Warning:
            nAndroidLogPriority = ANDROID_LOG_WARN;
            break;

          // Should be used for info texts
          case Info:
            nAndroidLogPriority = ANDROID_LOG_INFO;
            break;

          // Should be used for debug level texts
          case Debug:
            nAndroidLogPriority = ANDROID_LOG_DEBUG;
            break;
        }
        __android_log_write(nAndroidLogPriority, "BitrayEngine", sLogMessage);
      }
#else
      fputs(sLogMessage.c_str(), stdout);
#endif
    }

    // Is there a log formatter?
    bool bResult = true; // By default, all went fine
    if (m_pLogFormatter) {
      // Write the text
      bResult = m_pLogFormatter->output(nLogLevel, sText);
      if (bResult) {
        // Remove the latest message from the list if the limit is reached
        if (m_qLastMessages.size() == m_nBufferedMessages) {
          // Remove the oldest message
          m_qLastMessages.pop();
        }

        // Write the message in the queue
        if (m_nBufferedMessages && nLogLevel > Quiet)
          m_qLastMessages.push('(' + log_level_to_string(nLogLevel) + ") " + sText);

        // Should be the output buffer written to the file?
        m_nFlushMessages++;
        if (nLogLevel >= m_nFlushLogLevel || m_nFlushMessages >= m_nBufferedMessages) {
          m_nFlushMessages = 0;
          if (!m_pLogFormatter->flush())
            bResult = false; // Error!
        }

        // Emit the event
        EventNewEntry();
      }
    }

    // Done
    return bResult;
  }

  // Error!
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
