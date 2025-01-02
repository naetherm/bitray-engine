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
#include "core/tools/version.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
Version::Version()
: mMajor(0)
, mMinor(0)
, mPatch(0)
, mRelease("") {
}

Version::Version(const String &sName, const String &sCodename, uint16 nMajor, uint16 nMinor, uint16 nPatch, const String &sRelease)
: mName(sName)
, mCodename(sCodename)
, mMajor(nMajor)
, mMinor(nMinor)
, mPatch(nPatch)
, mRelease(sRelease) {
}

String Version::to_string() const {
  String sVersion;

  // "Name "
  if (mName.length())
    sVersion = mName + ' ';

  // "Major.Minor.Patch"
  sVersion += String() + static_cast<uint32>(mMajor) + '.' + static_cast<uint32>(mMinor) + '.' +
              static_cast<uint32>(mPatch);

  // "-release"
  if (mRelease.length())
    sVersion += '-' + mRelease;

  // " - Codename"
  if (mCodename.length())
    sVersion += " - " + mCodename;

  // Done
  return sVersion;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
bool Version::operator==(const Version &cVersion) const {
  return (mName == cVersion.mName &&
          mCodename == cVersion.mName &&
          mMajor == cVersion.mMajor &&
          mMinor == cVersion.mMinor &&
          mPatch == cVersion.mPatch &&
          mRelease == cVersion.mRelease);
}

bool Version::operator!=(const Version &cVersion) const {
  return (mName != cVersion.mName ||
          mCodename != cVersion.mName ||
          mMajor != cVersion.mMajor ||
          mMinor != cVersion.mMinor ||
          mPatch != cVersion.mPatch ||
          mRelease == cVersion.mRelease);
}

bool Version::operator<(const Version &cVersion) const {
  // Major
  if (mMajor < cVersion.mMajor)
    return true;
  else if (mMajor == cVersion.mMajor) {
    // Minor
    if (mMinor < cVersion.mMinor)
      return true;
    else if (mMinor == cVersion.mMinor) {
      // Patch
      if (mPatch < cVersion.mPatch)
        return true;
    }
  }
  return false;
}

bool Version::operator>(const Version &cVersion) const {
  // Major
  if (mMajor > cVersion.mMajor)
    return true;
  else if (mMajor == cVersion.mMajor) {
    // Minor
    if (mMinor > cVersion.mMinor)
      return true;
    else if (mMinor == cVersion.mMinor) {
      // Patch
      if (mPatch > cVersion.mPatch)
        return true;
    }
  }
  return false;
}

bool Version::operator<=(const Version &cVersion) const {
  // Major
  if (mMajor < cVersion.mMajor)
    return true;
  else if (mMajor == cVersion.mMajor) {
    // Minor
    if (mMinor < cVersion.mMinor)
      return true;
    else if (mMinor == cVersion.mMinor) {
      // Patch
      if (mPatch < cVersion.mPatch)
        return true;
    }
  }
  return false;
}

bool Version::operator>=(const Version &cVersion) const {
  // Major
  if (mMajor > cVersion.mMajor)
    return true;
  else if (mMajor == cVersion.mMajor) {
    // Minor
    if (mMinor > cVersion.mMinor)
      return true;
    else if (mMinor == cVersion.mMinor) {
      // Patch
      if (mPatch > cVersion.mPatch)
        return true;
    }
  }
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
