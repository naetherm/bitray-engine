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
#include "core/time/stopwatch.h"
#include "core/platform/platform.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Stopwatch::Stopwatch()
: mRunning(false)
, mStart(0)
, mStop(0) {
  // Nothing here
}

Stopwatch::Stopwatch(bool startAtOnce)
: mRunning(false)
, mStart(0)
, mStop(0) {
  if (startAtOnce) {
    start();
  }
}


void Stopwatch::start() {
  mRunning = true;
  mStart = Platform::instance().get_microseconds();
}

uint64 Stopwatch::stop() {
  // Is the stopwatch currently running?
  if (mRunning) {
    mStop = Platform::instance().get_microseconds();
    mRunning = false;
    return mStop - mStart;
  } else {
    return 0;
  }
}

uint64 Stopwatch::get_microseconds() const {
  return mRunning ? (Platform::instance().get_microseconds() - mStart) : (mStop - mStart);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
