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
//[ Public functions                                      ]
//[-------------------------------------------------------]
inline Version::~Version() {
}

inline String Version::get_name() const {
  return mName;
}

inline void Version::set_name(const String &sName) {
  mName = sName;
}

inline String Version::get_codename() const {
  return mCodename;
}

inline void Version::set_codename(const String &sCodename) {
  mCodename = sCodename;
}

inline uint16 Version::get_major() const {
  return mMajor;
}

inline void Version::set_major(uint16 nMajor) {
  mMajor = nMajor;
}

inline uint16 Version::get_minor() const {
  return mMinor;
}

inline void Version::set_minor(uint16 nMinor) {
  mMinor = nMinor;
}

inline uint16 Version::get_patch() const {
  return mPatch;
}

inline void Version::set_patch(uint16 nPatch) {
  mPatch = nPatch;
}

inline String Version::get_release() const {
  return mName;
}

inline void Version::set_release(const String &sRelease) {
  mRelease = sRelease;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
