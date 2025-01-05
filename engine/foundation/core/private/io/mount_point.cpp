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
#include "core/io/mount_point.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
MountPoint::MountPoint() {

}

MountPoint::MountPoint(const String& mount, const String& realPath)
: mMount(mount)
, mRealPath(realPath) {

}

MountPoint::MountPoint(const MountPoint& rhs)
: mMount(rhs.mMount)
, mRealPath(rhs.mRealPath) {

}

MountPoint::~MountPoint() {

}


MountPoint& MountPoint::operator=(const core::MountPoint &rhs) {
  mMount = rhs.mMount;
  mRealPath = rhs.mRealPath;

  return *this;
}

bool MountPoint::operator==(const core::MountPoint &rhs) const {
  return ((mMount == rhs.mMount) && (mRealPath == rhs.mRealPath));
}

bool MountPoint::operator!=(const core::MountPoint &rhs) const {
  return !operator==(rhs);
}


const String& MountPoint::get_mount() const {
  return mMount;
}

const String& MountPoint::get_real_path() const {
  return mRealPath;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
