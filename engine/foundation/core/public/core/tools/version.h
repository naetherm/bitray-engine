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
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Version class
 */
class Version {

  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   */
  CORE_API Version();

  /**
   * @brief
   * Constructor
   *
   * @param[in] sName
   * Project name
   * @param[in] sCodename
   * Code name for this release
   * @param[in] nMajor
   * Major version number
   * @param[in] nMinor
   * Minor version number
   * @param[in] nPatch
   * Patch number
   * @param[in] sRelease
   * release name (e.g. "r1" for release #1, "rc1" for release candidate #1, "nightly-date" for a nightly build etc.)
   */
  CORE_API Version(
    const String &sName,
    const String &sCodename,
    uint16 nMajor,
    uint16 nMinor,
    uint16 nPatch,
    const String &sRelease);

  /**
   * @brief
   * Destructor
   */
  inline ~Version();

  /**
   * @brief
   * get project name
   *
   * @return
   * Name of the project
   */
  inline String get_name() const;

  /**
   * @brief
   * Set project name
   *
   * @param[in] sName
   * Name of the project
   */
  inline void set_name(const String &sName);

  /**
   * @brief
   * get codename
   *
   * @return
   * Codename of the release
   */
  inline String get_codename() const;

  /**
   * @brief
   * Set codename
   *
   * @param[in] sCodename
   * Codename of the release
   */
  inline void set_codename(const String &sCodename);

  /**
   * @brief
   * get major version number
   *
   * @return
   * Major version
   */
  inline uint16 get_major() const;

  /**
   * @brief
   * Set major version number
   *
   * @param[in] nMajor
   * Major version
   */
  inline void set_major(uint16 nMajor);

  /**
   * @brief
   * get minor version number
   *
   * @return
   * Minor version
   */
  inline uint16 get_minor() const;

  /**
   * @brief
   * Set minor version number
   *
   * @param[in] nMinor
   * Minor version
   */
  inline void set_minor(uint16 nMinor);

  /**
   * @brief
   * get patch number
   *
   * @return
   * Patch number
   */
  inline uint16 get_patch() const;

  /**
   * @brief
   * Set patch number
   *
   * @param[in] nPatch
   * Patch number
   */
  inline void set_patch(uint16 nPatch);

  /**
   * @brief
   * get release name
   *
   * @return
   * release name
   */
  inline String get_release() const;

  /**
   * @brief
   * Set release name
   *
   * @param[in] sRelease
   * release name
   */
  inline void set_release(const String &sRelease);

  /**
   * @brief
   * Returns a string representation of the version
   *
   * @return
   * String representation (for example: FooBar 1.0.1-rc5 - Bulky Bug)
   */
  CORE_API String to_string() const;

  //[-------------------------------------------------------]
  //[ Comparison                                            ]
  //[-------------------------------------------------------]
  CORE_API bool operator==(const Version &cVersion) const;

  CORE_API bool operator!=(const Version &cVersion) const;

  CORE_API bool operator<(const Version &cVersion) const;

  CORE_API bool operator>(const Version &cVersion) const;

  CORE_API bool operator<=(const Version &cVersion) const;

  CORE_API bool operator>=(const Version &cVersion) const;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Project name */
  String mName;
  /** Code name for this release */
  String mCodename;
  /** Major version number */
  uint16 mMajor;
  /** Minor version number */
  uint16 mMinor;
  /** Patch number */
  uint16 mPatch;
  /** release name (e.g. "r1") */
  String mRelease;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/tools/version.inl"