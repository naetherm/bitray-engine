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
#include "core/container/vector.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class Path {
public:

  static Path get_current_dir();

public:

  Path();

  Path(const Path& rhs);

  Path(Path&& rhs);

  explicit Path(const String& rhs);

  explicit Path(String&& rhs);

  ~Path();


  Path& operator=(const Path& rhs);

  Path& operator=(Path&& rhs);

  Path& operator=(const String& rhs);

  Path& operator=(String&& rhs);

  Path& operator/=(const Path& other);

  Path& operator/=(const String& other);

  Path operator/(const Path& other) const;

  Path operator/(const String& other) const;


  void clear();


  Path& assign(const String& src);


  String get_url() const;

  String get_native_path() const;

  String get_windows_path() const;

  String get_unix_path() const;

  String get_protocol() const;

  String get_root() const;

  String get_path() const;

  String get_filename() const;

  String get_extension() const;

  String get_title() const;

  String get_complete_title() const;

  String cut_extension() const;

  String get_complete_extension() const;

  String cut_complete_extension() const;

  String get_path_part(uint32 position) const;

  uint32 get_num_path_parts() const;

  Path get_parent() const;


  bool is_valid() const;

  bool is_valid_native_path() const;

  bool is_valid_windows_path() const;

  bool is_valid_unix_path() const;

  bool is_empty() const;

  bool is_absolute() const;

  bool is_relative() const;

  bool is_directory() const;


  Path& collapse();

  void normalize();

private:

  void set_value(const String& path);

  bool is_name(const String& name) const;

  bool is_letter(char character) const;

  String get_path_from_pos(uint32& pathPos) const;

private:

  String mUrl;
  String mProtocol;
  String mRoot;
  String mPath;
  String mFilename;
  Vector<String> mPathParts;

  bool mValid;
  bool mDirty;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/io/path.inl"