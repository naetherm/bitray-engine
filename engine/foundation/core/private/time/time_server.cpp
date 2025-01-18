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
#include "core/time/time_server.h"
#include "core/platform/platform.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
TimeServer &TimeServer::instance() {
  static TimeServer SInstance;
  return SInstance;
}


void TimeServer::reset() {
  mLastFPSUpdateTime = mTimeLast = mTimeNow = mFPSLimitLastTime = Platform::instance().get_milliseconds();
}

bool TimeServer::check_update(core::uint64 *timeToWait) const {
  // By default, we don't need to wait
  if (timeToWait)
    *timeToWait = 0;

  // Is there a FPS limitation?
  if (mFPSLimit) {
    const uint64 nPastTime = Platform::instance().get_milliseconds() - mFPSLimitLastTime;
    const uint32 nTimePerFrame = static_cast<uint32>(1000 / mFPSLimit);
    if (nPastTime < nTimePerFrame) {
      if (timeToWait)
        *timeToWait = nTimePerFrame - nPastTime;

      // It's not time for an update!
      return false;
    }
  }

  // Done
  return true;
}

bool TimeServer::update(core::uint64 *timeToWait) {
  // Is the timing currently freezed?
  if (mFreezed)
    mTimeDifference = 0.0f;

  // Check FPS limitation
  if (mFPSLimit) {
    // Time for an update?
    if (check_update(timeToWait))
      mFPSLimitLastTime = Platform::instance().get_milliseconds();
    else
      return false; // Error - it's not time for an update
  } else {
    if (timeToWait)
      timeToWait = 0;
  }

  // Is the timing currently freezed?
  if (!mFreezed) {
    // Get time difference
    float32 fPrevTimeDifference = mTimeDifference;
    mTimeNow = Platform::instance().get_milliseconds();
    mTimeDifference = mTimeDifferenceNoCompensating = (mTimeNow - mTimeLast) / 1000.0f;
    if (!mActive || mTimeDifference > mMaxTimeDifference)
      mTimeDifference = mMaxTimeDifference;
    mTimeLast = mTimeNow;

    // Calculate the current FPS
    mPastFrames++;
    mFramesSinceCheck++;
    mFPSUpdateTimer += mTimeDifference;
    if (mFPSUpdateTimer > 1.0f) {
      mFPSUpdateTimer = 0.0f;
      mFramesPerSecond = static_cast<float32>(mFramesSinceCheck / ((mTimeNow - mLastFPSUpdateTime) / 1000.0f));
      if (mFramesPerSecond < 0.0f)
        mFramesPerSecond = 0.0f;
      mFramesSinceCheck = 0;
      mLastFPSUpdateTime = mTimeNow;
    }

    // Perform time scale
    mTimeDifference *= mTimeScaleFactor;
    mTimeDifferenceNoCompensating *= mTimeScaleFactor;

    // Perform slow motion
    if (mSlowMotion) {
      mTimeDifference *= mSlowMotionFactor * mCustomSlowMotionFactor;
      mTimeDifferenceNoCompensating *= mSlowMotionFactor * mCustomSlowMotionFactor;
    }

    // Try to compensate to abrupt timing slowdowns that may cause several problems
    float32 fDiff = mTimeDifference - fPrevTimeDifference;
    float32 fMaxStepDiff = mMaxTimeDifference / 3;
    if (fDiff > fMaxStepDiff)
      mTimeDifference = fMaxStepDiff + (fDiff - fMaxStepDiff) * 0.1f;

    // Update the general timing
    mTimer += mTimeDifference;
  }

  // Done
  return true;
}


core::uint64 TimeServer::get_past_time() const {
  return Platform::instance().get_milliseconds() - mTimeStart;
}


void TimeServer::freeze(bool freeze) {
  if (freeze) {
    if (!mFreezed) {
      mFreezed = true;
      mFreezeTime = mTimeNow;
    }
  } else {
    if (mFreezed) { // Update timing
      const uint64 nTimeDifference = Platform::instance().get_milliseconds() - mFreezeTime;
      mTimeLast += nTimeDifference;
      mLastFPSUpdateTime += nTimeDifference;
    }
    mFreezed = false;
  }
}


TimeServer::TimeServer()
: mTimeStart(Platform::instance().get_milliseconds())
, mTimeNow(mTimeStart)
, mTimeLast(mTimeStart)
, mLastFPSUpdateTime(mTimeStart)
, mFreezeTime(0)
, mFPSLimitLastTime(mTimeStart)
, mTimeDifference(0.0f)
, mTimeDifferenceNoCompensating(0.0f)
, mMaxTimeDifference(0.15f)
, mFramesPerSecond(0.0f)
, mFramesSinceCheck(0)
, mPastFrames(0)
, mFPSUpdateTimer(0.0f)
, mTimeScaleFactor(1.0f)
, mSlowMotionFactor(1.0f)
, mCustomSlowMotionFactor(1.0f)
, mFPSLimit(0.0f)
, mTimer(0.0f)
, mPause(false)
, mActive(true)
, mFreezed(false)
, mSlowMotion(true) {

}

TimeServer::~TimeServer() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}