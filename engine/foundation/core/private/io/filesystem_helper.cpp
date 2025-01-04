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
#include "core/io/filesystem_helper.h"
#include "core/io/filesystem_impl.h"
#include "core/io/file_impl.h"
#include "core/math/math.h"
#include "core/string/string_splitter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
String FilesystemHelper::normalize(String path) {
  String result(path);

  // Correct all '//' to '/'
  result.replace_all("//", "/");
  result.replace_all(":/", ":");

  // Correct path further, e.g. remove all "." and ".."
  Vector<String> splitPath = StringSplitter::split(result, "/");

  /// TODO(naetherm): Dirty hack for now
  if (result[0] == '/') {
    splitPath.insert_at(0, "");
  }

  // Find ".." and remove them
  Vector<uint32> parentPositions;
  for (uint32 i = 0; i < splitPath.size(); ++i) {
    if (splitPath[i] == "..") {
      parentPositions.push_back(i);
    }
  }
  // And remove all occurrences and the previous element as well
  for (auto element: parentPositions) {
    splitPath[element] = ".";
    splitPath[element - 1] = ".";
  }

  // Remove "."
  for (int32 i = splitPath.size() - 1; i >= 0; --i) {
    if (splitPath[i] == ".") {
      splitPath.erase_at(i);
    }
  }

  result = build_string(splitPath, "/");

  return result;
}

String FilesystemHelper::build_string(const Vector<String> &strings, String fence) {
  String result;

  if (strings.size() > 0) {
    result += strings[0];

    for (int32 i = 1; i < strings.size(); ++i) {
      result += fence + strings[i];
    }
  }

  return result;
}

String FilesystemHelper::read_file_as_string(const FilesystemImpl &filesystem, const String &filename) {
  FileImpl* file = filesystem.open_file(EFileMode::Read, filename);

  String result;

  if (file) {
    uint32 sizeInBytes = file->get_number_of_bytes();

    result.resize(sizeInBytes);

    file->read(result.data(), sizeInBytes);

    filesystem.close_file(*file);
  }

  return result;
}

String FilesystemHelper::read_file_as_string(FileImpl& file) {
  String result;

  uint32 sizeInBytes = file.get_number_of_bytes();

  result.resize(sizeInBytes);

  file.read(result.data(), sizeInBytes);

  return result;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
