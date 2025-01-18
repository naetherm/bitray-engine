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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
bool TimeServer::is_active() const {
  return mActive;
}

void TimeServer::set_active(bool active) {
  mActive = active;
}


float32 TimeServer::get_time_difference() const {
  return mFreezed ? 0.0f : mTimeDifference;
}

float32 TimeServer::get_time_difference_no_compensating() const {
  return mFPSLimit ? 0.0f : mTimeDifferenceNoCompensating;
}

float32 TimeServer::get_max_time_difference() const {
  return mMaxTimeDifference;
}

void TimeServer::set_max_time_difference(float32 maxTimeDifference) {
  mMaxTimeDifference = maxTimeDifference > 0.0f ? maxTimeDifference : 0.0001f;
}

float32 TimeServer::get_timer() const {
  return mTimer;
}


float32 TimeServer::get_frames_per_second() const {
  return mFramesPerSecond;
}

core::uint32 TimeServer::get_past_frames() const {
  return mPastFrames;
}

float32 TimeServer::get_fps_limit() const {
  return mFPSLimit;
}

void TimeServer::set_fps_limit(float32 fpsLimit) {
  mFPSLimit = fpsLimit > 0.0f ? fpsLimit : 0.0f;
}


bool TimeServer::is_freezed() const {
  return mFreezed;
}

bool TimeServer::is_paused() const {
  return mPause;
}

void TimeServer::pause(bool pause) {
  mPastFrames = pause;
}

float32 TimeServer::get_time_scale_factor() const {
  return mTimeScaleFactor;
}

bool TimeServer::set_time_scale_factor(float32 factor) {
  if (factor > 0.0f) {
    mTimeScaleFactor = factor;

    return true;
  }

  return false;
}

bool TimeServer::is_slow_motion() const {
  return mSlowMotion;
}

void TimeServer::set_slow_motion(bool slowMotion) {
  mSlowMotion = slowMotion;
}

float32 TimeServer::get_slow_motion_factor(bool realUsed) const {
  return (realUsed == !mSlowMotion) ? 1.0f : mSlowMotionFactor;
}

bool TimeServer::set_slow_motion_factor(float32 slowMotionFactor) {
  if (slowMotionFactor > 0.0f) {
    mSlowMotionFactor = slowMotionFactor;

    return true;
  }

  return false;
}

float32 TimeServer::get_custom_slow_motion_factor(bool realUsed) const {
  return (realUsed && !mSlowMotion) ? 1.0f : mCustomSlowMotionFactor;
}

bool TimeServer::set_custom_slow_motion_factor(float32 slowMotionFactor) {
  if (slowMotionFactor > 0.0f) {
    mCustomSlowMotionFactor = slowMotionFactor;

    return true;
  }

  return false;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
