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
#include <ctime>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Stopwatch
 *
 * @brief
 * Stopwatch
 *
 * @verbatim
 * Usage example:
 *
 *   // start the stopwatch
 *   Stopwatch stopwatch(true);
 *
 *   // Do some stuff
 *
 *   // Measure elapsed time
 *   float elapsedSeconds = stopwatch.get_seconds();
 * @endverbatim
 */
class Stopwatch {
  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Default constructor
   */
  Stopwatch();

  /**
   * @brief
   * Constructor
   *
   * @param[in] startAtOnce
   * If this parameter is "true", the stopwatch is started automatically at once
   */
  explicit Stopwatch(bool startAtOnce);

  /**
   * @brief
   * Destructor
   */
  ~Stopwatch() = default;

  /**
   * @brief
   * start the stopwatch
   *
   * @note
   * - If the stopwatch is already running it's restarted
   */
  void start();

  /**
   * @brief
   * stop the stopwatch
   *
   * @return
   * The elapsed time in microseconds since "start()"
   *
   * @note
   * - Often it's adequate to just request the pastime using
   *   e.g. "get_milliseconds()" and not explicitly stopping the stopwatch
   */
  uint64 stop();

  /**
   * @brief
   * Return the number of weeks since the stopwatch was started
   *
   * @return
   * Number of weeks elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float get_weeks() const;

  /**
   * @brief
   * Return the number of days since the stopwatch was started
   *
   * @return
   * Number of days elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float get_days() const;

  /**
   * @brief
   * Return the number of hours since the stopwatch was started
   *
   * @return
   * Number of hours elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float get_hours() const;

  /**
   * @brief
   * Return the number of minutes since the stopwatch was started
   *
   * @return
   * Number of minutes elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float get_minutes() const;

  /**
   * @brief
   * Return the number of seconds since the stopwatch was started
   *
   * @return
   * Number of seconds elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float get_seconds() const;

  /**
   * @brief
   * Return the number of milliseconds since the stopwatch was started
   *
   * @return
   * Number of milliseconds elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float get_milliseconds() const;

  /**
   * @brief
   * Retrieve the number of microseconds since the stopwatch was started
   *
   * @return
   * Number of microseconds elapsed since the stopwatch was started
   */
  [[nodiscard]] uint64 get_microseconds() const;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Is the stopwatch currently running? */
  bool mRunning;
  /** Stopwatch start time (microseconds) */
  uint64 mStart;
  /** Stopwatch stop time (microseconds) */
  uint64 mStop;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/time/stopwatch.inl"