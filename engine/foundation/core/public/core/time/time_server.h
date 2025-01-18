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
#include "core/core/server_impl.h"


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
 * TimeServer
 *
 * @brief
 * TimeServer class offers a lot of timing tool functions you can for
 * instance use to create timed movement. Have a look at the 'Platform'-singleton
 * for the basic timing functions.
 */
class TimeServer : public ServerImpl {
public:
  /**
   * @brief
   * Returns the static and unique singleton instance.
   *
   * @return
   * The static and unique singleton instance.
   */
  static TimeServer &instance();

public:

  /**
   * @brief
   * Returns whether the timing is active or not
   *
   * @return
   * 'true' if the timing is active, else 'false'
   */
  inline bool is_active() const;

  /**
   * @brief
   * Sets whether the timing is active or not
   *
   * @param[in] bActive
   * Should the timing be active?
   *
   * @note
   * - If the timing is disabled the time difference between
   *   two frames will be always the maximum time difference
   * - Use it only for testing
   */
  inline void set_active(bool active = true);

  /**
   * @brief
   * Resets the timing
   */
  void reset();

  /**
   * @brief
   * Checks whether the timing has to be updated or not
   *
   * @param[in] pnTimeToWait
   * If not a null pointer, if there's a FPS limit and it's not time for an update -> receives the time in
   * milliseconds we need to wait until it's time for the next update, receives '0' in all other cases
   *
   * @return
   * 'true' if the timing has to be updated, else 'false'
   *
   * @remarks
   * Normally the timing must be updated every frame. But if theres a frame
   * rate limitation it's not allowed to update the timing every frame!
   */
  bool check_update(core::uint64 *timeToWait = nullptr) const;

  /**
   * @brief
   * Updates all time relevant stuff
   *
   * @param[in] pnTimeToWait
   * If not a null pointer, if there's a FPS limit and it's not time for an update -> receives the time in
   * milliseconds we need to wait until it's time for the next update, receives '0' in all other cases
   *
   * @return
   * 'true' if all went fine, else 'false' (maybe there's a frame rate limitation)
   *
   * @see
   * - check_update()
   */
  bool update(core::uint64 *timeToWait = nullptr);


  /**
   * @brief
   * Returns the past time since last frame (seconds)
   *
   * @return
   * Past time since last frame
   *
   * @remarks
   * The timing class tries to compensate irregularities in timing automatically to
   * avoid for instance sudden jumps through half of the universe. Further a maximum
   * time difference is applied to avoid any totally undesired results.
   */
  inline float32 get_time_difference() const;

  /**
   * @brief
   * Returns the past time since last frame without any compensating (seconds)
   *
   * @return
   * Past time since last frame without any compensating
   *
   * @note
   * - Use get_time_difference() instead this function whenever you can
   */
  inline float32 get_time_difference_no_compensating() const;

  /**
   * @brief
   * Returns the maximum time difference
   *
   * @return
   * The maximum time difference since the last frame in seconds
   *
   * @note
   * - The maximum time difference will avoid a too height time difference
   *   value which would probably end in undefined problems.
   * - A maximum time difference of 0.15 is a good value
   */
  inline float32 get_max_time_difference() const;

  /**
   * @brief
   * Sets the maximum time difference
   *
   * @param[in] fMaxTimeDifference
   * The maximum time difference since the last frame in seconds (> 0)
   *
   * @see
   * - get_max_time_difference()
   */
  inline void set_max_time_difference(float32 maxTimeDifference = 0.15f);

  /**
   * @brief
   * Returns the past time in milliseconds since the application start
   *
   * @return
   * Past time in milliseconds since the application start
   */
  core::uint64 get_past_time() const;

  /**
   * @brief
   * Returns a general timing
   *
   * @return
   * General timing
   *
   * @remarks
   * On each timing update, the current time difference is added to this general timing.
   */
  inline float32 get_timer() const;


  /**
   * @brief
   * Returns the current frames per second (FPS)
   *
   * @return
   * Current frames per second
   */
  inline float32 get_frames_per_second() const;

  /**
   * @brief
   * Returns the number of past frames since timing start
   *
   * @return
   * Current frames per second (FPS)
   */
  inline core::uint32 get_past_frames() const;

  /**
   * @brief
   * Returns the FPS limit
   *
   * @return
   * FPS limit, 0 if there's no FPS limitation
   *
   * @note
   * - FPS limitation could be used to avoid that the application
   *   uses to much system resources. (normally 30 FPS are enough)
   * - Use a FPS limitation only in special situations!
   *   (users love FPS over 100 even if its more or less senseless ;-)
   */
  inline float32 get_fps_limit() const;

  /**
   * @brief
   * Sets the FPS limit
   *
   * @param[in] fFPSLimit
   * FPS limit, 0 if there should be no limitation (>= 0)
   *
   * @see
   * - fet_fps_limit()
   */
  inline void set_fps_limit(float32 fpsLimit = 0.0f);


  /**
   * @brief
   * Returns if the timing is currently freezed or not
   *
   * @return
   * 'true' if the timing is freezed at the moment, else 'false'
   */
  inline bool is_freezed() const;

  /**
   * @brief
   * Set freezed mode
   *
   * @param[in] bFreeze
   * Should the timing be freezed or not?
   *
   * @note
   * - Freeze your application if you are not in rendering mode
   *   (e.g. you load something)
   */
  void freeze(bool freeze = true);

  /**
   * @brief
   * Returns whether the timing is paused of not
   *
   * @return
   * 'true' if the timing is paused, else 'false'
   *
   * @note
   * - If the timing is paused scene nodes, particles etc. are not updated
   * - The timing will still be updated
   */
  inline bool is_paused() const;

  /**
   * @brief
   * Set pause mode
   *
   * @param[in] bPause
   * Should the timing be paused or not?
   */
  inline void pause(bool pause = true);

  /**
   * @brief
   * Returns the time scale factor
   *
   * @return
   * Time scale factor
   *
   * @note
   * - The global time scale factor should only be manipulated for debugging
   * - For 'slow motion' effects use the slow motion functions instead!
   * - A factor of <= 0 is NOT allowed because this may cause problems in certain
   *   situations, pause the timer instead by hand!
   * - Do NOT make the factor 'too' (for example > 4) extreme, this may cause
   *   problems in certain situations!
   */
  inline float32 get_time_scale_factor() const;

  /**
   * @brief
   * Sets the time scale factor
   *
   * @param[in] fFactor
   * Time scale, a factor of <= 0 is NOT allowed!
   *
   * @return
   * 'true' if all went fine, else 'false' (maybe the given factor is <= 0?)
   *
   * @see
   * - get_time_scale_factor()
   */
  inline bool set_time_scale_factor(float32 factor = 1.0f);

  /**
   * @brief
   * Returns if the slow motion is activated or not
   *
   * @return
   * 'true' is the slow motion mode is activated, else 'false'
   */
  inline bool is_slow_motion() const;

  /**
   * @brief
   * Activates/deactivates the slow motion mode
   *
   * @param[in] bSlowMotion
   * Should the slow motion mode be activated?
   */
  inline void set_slow_motion(bool slowMotion = false);

  /**
   * @brief
   * Returns the slow motion factor
   *
   * @param[in] bRealUsed
   * The real used slow motion factor will be returned. If the slow motion mode
   * is deactivated this will be 1!
   *
   * @return
   * The current slow motion factor
   *
   * @note
   * - The slow motion mode must be activated that this factor
   *   changes the game speed
   * - Use this function to change the speed in general
   * - A factor of <= 0 is NOT allowed because this may cause problems in certain
   *   situations, pause the timer instead by hand!
   * - Do NOT make the factor 'too' (for example > 4) extreme, this may cause
   *   problems in certain situations!
   */
  inline float32 get_slow_motion_factor(bool realUsed = true) const;

  /**
   * @brief
   * Sets the slow motion factor
   *
   * @param[in] fSlowMotionFactor
   * The slow motion factor, a factor of <= 0 is NOT allowed!
   *
   * @return
   * 'true' if all went fine, else 'false' (maybe the given factor is <= 0?)
   *
   * @see
   * - get_slow_motion_factor()
   */
  inline bool set_slow_motion_factor(float32 slowMotionFactor = 1.0f);

  /**
   * @brief
   * Returns the custom slow motion factor
   *
   * @param[in] bRealUsed
   * The real used slow motion factor will be returned. If the slow motion mode
   * is deactivated this will be 1!
   *
   * @return
   * The current slow motion factor
   *
   * @note
   * - The slow motion mode must be activated that this
   *   factor changes the game speed
   * - Use this factor to temporal slow motion effects
   */
  inline float32 get_custom_slow_motion_factor(bool realUsed = true) const;

  /**
   * @brief
   * Sets the custom slow motion factor
   *
   * @param[in] fSlowMotionFactor
   * The slow motion factor, a factor of <= 0 is NOT allowed!
   *
   * @return
   * 'true' if all went fine, else 'false' (maybe the given factor is <= 0?)
   *
   * @see
   * - get_custom_slow_motion_factor()
   */
  inline bool set_custom_slow_motion_factor(float32 slowMotionFactor = 1.0f);

protected:
  /**
   * @brief
   * Constructor.
   */
  TimeServer();

  /**
   * @brief
   * Destructor.
   */
  ~TimeServer();

private:
  /** The start system time */
  uint64 mTimeStart;
  /** Current system time */
  uint64 mTimeNow;
  /** System time of the last frame */
  uint64 mTimeLast;
  /** The time were the last FPS update was done */
  uint64 mLastFPSUpdateTime;
  /** The time were the application was frozen */
  uint64 mFreezeTime;
  /** Time were the last update was done */
  uint64 mFPSLimitLastTime;
  /** The past time since last frame (seconds) */
  float32 mTimeDifference;
  /** The past time since last frame without any compensating (seconds) */
  float32 mTimeDifferenceNoCompensating;
  /** The maximum possible time difference (seconds) */
  float32 mMaxTimeDifference;
  /** Current frames per second (FPS... hopefully not SPS... Seconds Per Frame ;) */
  float32 mFramesPerSecond;
  /** Number of frames since last FPS update */
  int mFramesSinceCheck;
  /** Number of past frames since timing start */
  uint32 mPastFrames;
  /** Timing for updating FPS (only each second) */
  float32 mFPSUpdateTimer;
  /** Time scale factor */
  float32 mTimeScaleFactor;
  /** The slow motion factor */
  float32 mSlowMotionFactor;
  /** The custom slow motion factor */
  float32 mCustomSlowMotionFactor;
  /** FPS limitation */
  float32 mFPSLimit;
  /** General timing */
  float32 mTimer;

  /** Is the timing paused? */
  bool mPause;
  /** Is the timing active? */
  bool mActive;
  /** Is the application currently freezed? */
  bool mFreezed;
  /** Is the slow motion modus activated? */
  bool mSlowMotion;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/time/time_server.inl"