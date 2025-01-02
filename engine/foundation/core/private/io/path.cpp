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
#include "core/io/path.h"
#include "core/container/stack.h"
#include "core/platform/platform.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Path Path::get_current_dir() {
  return Path(Platform::instance().get_current_dir());
}


Path::Path()
: mValid(false)
, mDirty(false) {

}

Path::Path(const Path& rhs)
: mUrl(rhs.mUrl)
, mProtocol(rhs.mProtocol)
, mRoot(rhs.mRoot)
, mPath(rhs.mPath)
, mFilename(rhs.mFilename)
, mPathParts(rhs.mPathParts)
, mValid(rhs.mValid)
, mDirty(rhs.mDirty) {

}

Path::Path(Path&& rhs)
: mUrl(rhs.mUrl)
, mProtocol(rhs.mProtocol)
, mRoot(rhs.mRoot)
, mPath(rhs.mPath)
, mFilename(rhs.mFilename)
, mPathParts(rhs.mPathParts)
, mValid(rhs.mValid)
, mDirty(rhs.mDirty)  {

}

Path::Path(const String& rhs) {
  set_value(rhs);
}

Path::Path(String&& rhs) {
  set_value(rhs);
}

Path::~Path() {

}


Path& Path::operator=(const Path& rhs) {
  mUrl = rhs.mUrl;
  mProtocol = rhs.mProtocol;
  mRoot = rhs.mRoot;
  mPath = rhs.mPath;
  mFilename = rhs.mFilename;
  mPathParts = rhs.mPathParts;
  mValid = rhs.mValid;
  mDirty = rhs.mDirty;

  return *this;
}

Path& Path::operator=(Path&& rhs) {
  mUrl = rhs.mUrl;
  mProtocol = rhs.mProtocol;
  mRoot = rhs.mRoot;
  mPath = rhs.mPath;
  mFilename = rhs.mFilename;
  mPathParts = rhs.mPathParts;
  mValid = rhs.mValid;
  mDirty = rhs.mDirty;

  return *this;
}

Path& Path::operator=(const String& rhs) {
  set_value(rhs);

  return *this;
}

Path& Path::operator=(String&& rhs) {
  set_value(rhs);

  return *this;
}

Path& Path::operator/=(const Path& other) {
  if (is_directory()) {
    if (other.is_relative()) {
      mPath += other.mPath;
      mPathParts.push_back(other.mPathParts);
      mFilename = other.mFilename;
      mUrl += other.mPath + mFilename;
    }
  }
  return *this;
}

Path& Path::operator/=(const String& other) {
  if (is_directory()) {
    String temp;
    if (!other.contains(".") && (other[other.length()-1] != '/')) {
      temp = other + "/";
    } else {
      temp = other;
    }
    Path path(temp);
    if (path.is_relative()) {
      mPath += path.mPath;
      if (other[other.length()-1] != '/') {
        mPath += '/';
      }
      mPathParts.push_back(path.mPathParts);
      mFilename = path.mFilename;
      mUrl += path.mPath + mFilename;
    }
  }
  return *this;
}

Path Path::operator/(const Path& other) const {
  if (is_directory()) {
    Path path(*this);
    if (other.is_relative()) {
      String newPath;
      for (core::uint32 i = 0; i < mPathParts.size(); ++i) {
        newPath += mPathParts[i] + "/";
      }
      for (core::uint32 i = 0; i < other.get_num_path_parts(); ++i) {
        newPath += other.get_path_part(i) + "/";
      }
      if (!other.is_directory()) {
        newPath += other.get_filename();
      }

      return Path(newPath);
    }
  }

  return Path(*this);
}

Path Path::operator/(const String& other) const {
  if (is_directory()) {
    Path path(*this);
    String temp;
    if (!other.contains(".") && (other[other.length()-1] != '/')) {
      temp = other + "/";
    } else {
      temp = other;
    }
    Path otherPath(temp);
    if (otherPath.is_relative()) {
      String newPath;
      for (core::uint32 i = 0; i < mPathParts.size(); ++i) {
        newPath += mPathParts[i] + "/";
      }
      for (core::uint32 i = 0; i < otherPath.get_num_path_parts(); ++i) {
        newPath += otherPath.get_path_part(i) + "/";
      }
      if (!otherPath.is_directory()) {
        newPath += otherPath.get_filename();
      }

      return Path(newPath);
    }
  }

  return Path(*this);
}


void Path::clear() {
  mUrl = "";
  mProtocol = "";
  mRoot = "";
  mPath = "";
  mFilename = "";
  mValid = false;
}


Path& Path::assign(const String& src) {
  operator/=(src);

  return *this;
}


String Path::get_url() const {
  return mUrl;
}

String Path::get_native_path() const {
#if defined(LINUX)
  return get_unix_path();
#else
  return get_windows_path();
#endif
}

String Path::get_windows_path() const {
  // Check whether the protocol is correct
  if ((mProtocol.compare("file://") == 0 || mProtocol.empty())) {
    String path = mRoot + mPath + mFilename;
    path = path.replace_all("/", "\\");
    return path;
  } else {
    return get_url();
  }
}

String Path::get_unix_path() const {
  // Check whether the protocol is correct
  if ((mProtocol.compare("file://") == 0 || mProtocol.empty())) {
    String path = mRoot + mPath + mFilename;
    return path;
  } else {
    return get_url();
  }
}

String Path::get_protocol() const {
  return mProtocol;
}

String Path::get_root() const {
  return mRoot;
}

String Path::get_path() const {
  return mPath;
}

String Path::get_filename() const {
  return mFilename;
}

String Path::get_extension() const {
  return mFilename.substr(mFilename.find_last_of("."));
}

String Path::get_title() const {
  return mFilename.substr(0, mFilename.find_first_of("."));
}

String Path::get_complete_title() const {
  return mFilename.substr(0, mFilename.find_last_of("."));
}

String Path::cut_extension() const {
  String path;
  if (!mProtocol.comparei("file://") && mProtocol.length()) {
    path = mProtocol;
  }
  path += mRoot + mPath + get_complete_title();
  return path;
}

String Path::get_complete_extension() const {
  return mFilename.substr(mFilename.find_first_of("."));
}

String Path::cut_complete_extension() const {
  String path;
  if (!mProtocol.comparei("file://") && mProtocol.length()) {
    path = mProtocol;
  }
  path += mRoot + mPath + get_title();
  return path;
}

String Path::get_path_part(uint32 position) const {
  return mPathParts[position];
}

uint32 Path::get_num_path_parts() const {
  return mPathParts.size();
}

Path Path::get_parent() const {
  Path result(*this);

  if (is_directory()) {
    // Just remove the last element
    // Now shorten the paths
    result.mPath = result.mPath.substr(0, result.mPath.find_last_of("/"));
    result.mPathParts.erase_at(result.mPathParts.size()-1);
  }

  return result;
}


bool Path::is_valid() const {
  return mValid;
}

bool Path::is_valid_native_path() const {
#if defined(LINUX)
  return is_valid_unix_path();
#else
  return is_valid_windows_path();
#endif
}

bool Path::is_valid_windows_path() const {
  return ((mProtocol.comparei("file://") == 0 || mProtocol.empty()) &&
    (mRoot.substr(1, 2) == ":/" || mRoot.empty()));
}

bool Path::is_valid_unix_path() const {
  return ((mProtocol.comparei("file://") == 0 || mProtocol.empty()) &&
          (mRoot == "/" || mRoot.empty()));
}

bool Path::is_empty() const {
  return (mValid && mRoot.empty() && mPath.empty() && mFilename.empty());
}

bool Path::is_absolute() const {
  return (mValid && mRoot.length());
}

bool Path::is_relative() const {
  return (mValid && !mRoot.length());
}

bool Path::is_directory() const {
  return (mValid && (mRoot.length() || mPath.length()) && !mFilename.length());
}


Path& Path::collapse() {
  if (mValid) {
    // Validate paths (Resolve paths like '..' and '.')
    Stack<String> cNameStack;
    uint32 nPos = 0;
    String sName = get_path_from_pos(nPos);
    while (sName.length()) {
      // Check next subdir
      const String sTop = cNameStack.top();
      if (sName.length()) {
        if (sName == ".." && sTop != ".." && (sTop.length() || mRoot.length())) {
          // Delete last subdir when '..' occurs
          // Note: m_sRoot.length() is there because e.g. "C:\.." is a valid directory on Windows systems.
          // To avoid directory names like C:\..\..\Programs\", this function will therefore ignore
          // ".." at the beginning of a filename, but ONLY if the root is not empty (relative paths
          // can of cause begin with "..")
          if (sTop.length())
            cNameStack.pop();
        } else if (sName == ".") {
          // Ignore "."
        } else {
          // Push subdir on stack
          cNameStack.push(sName);
        }
      }

      // Read next
      sName = get_path_from_pos(nPos);
    }

    // Look for .. or . in the filename
    const String sTop = cNameStack.top();
    if (mFilename == ".." && sTop != ".." && (sTop.length() || mRoot.length())) {
      cNameStack.pop();
      mFilename = cNameStack.top();
      cNameStack.pop();
    } else if (mFilename == ".") {
      mFilename = cNameStack.top();
      if (!mFilename.length())
        mFilename = ".";
      cNameStack.pop();
    }

    // Put path together
    mPath = "";
    mPathParts.clear();
    while (cNameStack.size() > 0) {
      String pathPart = cNameStack.top();
      mPath = pathPart + "/" + mPath;
      mPathParts.push_back(pathPart);
      cNameStack.pop();
    }

    // Compose new path or URL
    mUrl = mProtocol + mRoot + mPath + mFilename;
  }
  
  return *this;
}

void Path::normalize() {
  // Rebuild the path
  core::Vector<core::uint32> indexes;
  for (core::uint32 i = 0; i < mPathParts.size(); ++i) {
    if (mPathParts[i] == ".") {
      indexes.push_back(i);
    }
  }

  for (core::int32 i = indexes.size() - 1; i >= 0; --i) {
    mPathParts.erase_at(indexes[i]);
  }

  // Now remove all ".."
  while (mPathParts.contains("..")) {
    for (core::uint32 i = 0; i < mPathParts.size(); ++i) {
      if (mPathParts[i] == "..") {
        if (i > 0) {
          mPathParts.erase_at(i);
          mPathParts.erase_at(i - 1);
        }
        break;
      }
    }
  }

  // Rebuild mPath
  mPath = "";
  for (core::uint32 i = 0; i < mPathParts.size(); ++i) {
    mPath += mPathParts[i] + "/";
  }
}


void Path::set_value(const String& path) {
  // First clear old path
  clear();

  // First check some edge cases
  if (!path.empty()) {
    String temp = path;

    // First, check for the protocol that is used, e.g. "http://", "file://", etc.
    String protocol, root;
    sizeT position = temp.find("://");
    if (position >= 2 && position != String::NPOS) {
      protocol = path.substr(0, position);
      if (!is_name(protocol)) {
        return;
      }

      protocol += "://";
      protocol.make_lower();
      temp = temp.substr(protocol.length());
    } else {

    }

    // Check for Linux file root, e.g. on linux "/"
    if (root.empty() && (temp[0] == '/' || temp[0] == '\\')) {
      root = temp[0];
      temp = temp.substr(1);
    }

    // Windows specific: check if there is a drive letter
    if (is_letter(temp[0]) && temp[1] == ':' && (temp[2] == '/' || temp[2] == '\\')) {
      root = temp.substr(0, 1).to_upper() + ":/";
      temp = temp.substr(3);
    }

    // If no protocol was found let's set it to "file://"
    if (protocol.empty() && !root.empty()) {
      protocol = "file://";
    }

    // Check path
    String path, filename;
    Vector<String> pathParts;
    while (temp.length()) {
      const sizeT backs = temp.find('\\');
      sizeT slash = temp.find('/');
      if (slash == String::NPOS || (backs != String::NPOS && backs < slash)) {
        slash = backs;
      }

      // Check name
      const String name = (slash != String::NPOS) ? temp.substr(0, slash) : temp;
      if (is_name(name)) {
        if (slash != String::NPOS) {
          // Compose path
          path += name + '/';
          pathParts.push_back(name);
        } else {
          filename = temp;
        }
      } else {
        // Ignore empty names
      }

      // Get next part
      if (slash != String::NPOS) {
        temp = temp.substr(slash + 1);
      } else {
        temp = "";
      }
    }

    // Last checks
    if (root != "/" || mUrl.find('\\') == String::NPOS) {
      mValid = true;
      mProtocol = protocol;
      mRoot = root;
      mPath = path;
      mPathParts = pathParts;
      mFilename = filename;
      mUrl = protocol + root + path + filename;
    }
  }
}

bool Path::is_name(const String& name) const {
  //return (name.length() && !name.contains(":") && !name.contains("/") && !name.contains("\\"));
  return (name.length() && !name.contains("/") && !name.contains("\\"));
}

bool Path::is_letter(char character) const {
  // Check if the character is a letter
  return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'));
}

String Path::get_path_from_pos(uint32 &pathPos) const {
  const String temp = mPath.substr(pathPos);
  const int nBacks = temp.find_first_of('\\');
  int nSlash = temp.find_first_of('/');
  if (nSlash < 0 || (nBacks >= 0 && nBacks < nSlash))
    nSlash = nBacks;
  if (nSlash >= 0) {
    pathPos += nSlash + 1;
    return temp.substr(0, nSlash);
  } else {
    pathPos += temp.length();
    return "";
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
