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
#include "io/path.h"
#include <core/io/path.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
PathTests::PathTests()
  : UnitTest("PathTests") {

}

PathTests::~PathTests() {

}

void PathTests::test() {
  {
    core::String strPath("/some/directory/");

    be_expect_str_eq("/some/directory/", strPath.c_str())
    core::Path path(strPath);
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/", path.get_path().c_str())
    be_expect_true(path.get_filename().empty())
    be_expect_true(path.is_directory())
    be_expect_true(path.is_absolute())
    be_expect_false(path.is_relative())
    be_expect_true(path.is_valid())
    be_expect_true(path.is_valid_native_path())
    be_expect_true(path.is_valid_unix_path())
    be_expect_eq(strlen("file:///some/directory/"), path.get_url().length())
    be_expect_str_eq("file:///some/directory/", path.get_url().c_str())
    be_expect_str_eq("/some/directory/", path.get_native_path().c_str())
  }
  {
    core::String strPath("/some/directory/file.txt");

    be_expect_str_eq("/some/directory/file.txt", strPath.c_str())
    core::Path path(strPath);
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/", path.get_path().c_str())
    be_expect_str_eq("file.txt", path.get_filename().c_str())
    be_expect_false(path.is_directory())
    be_expect_true(path.is_absolute())
    be_expect_false(path.is_relative())
    be_expect_true(path.is_valid())
    be_expect_true(path.is_valid_native_path())
    be_expect_true(path.is_valid_unix_path())
    be_expect_str_eq("file:///some/directory/file.txt", path.get_url().c_str())
  }
  {
    core::String strPath("/some/directory/");
    core::String otherStrPath("subdirectory/content/");

    be_expect_str_eq("/some/directory/", strPath.c_str())
    be_expect_str_eq("subdirectory/content/", otherStrPath.c_str())
    core::Path path(strPath);
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/", path.get_path().c_str())
    core::Path otherPath(otherStrPath);
    be_expect_str_eq("subdirectory/content/", otherPath.get_url().c_str())

    path /= otherPath;
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/subdirectory/content/", path.get_path().c_str())
    be_expect_str_eq("file:///some/directory/subdirectory/content/", path.get_url().c_str())
  }
  {
    core::String strPath("/some/directory/");
    core::String otherStrPath("subdirectory/content/foobar.asset");

    be_expect_str_eq("/some/directory/", strPath.c_str())
    be_expect_str_eq("subdirectory/content/foobar.asset", otherStrPath.c_str())
    core::Path path(strPath);
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/", path.get_path().c_str())
    core::Path otherPath(otherStrPath);
    be_expect_str_eq("subdirectory/content/foobar.asset", otherPath.get_url().c_str())

    path /= otherPath;
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/subdirectory/content/", path.get_path().c_str())
    be_expect_str_eq("file:///some/directory/subdirectory/content/foobar.asset", path.get_url().c_str())
  }
  {
    core::String strPath("/some/directory/");
    core::String otherStrPath("/subdirectory/content/");

    be_expect_str_eq("/some/directory/", strPath.c_str())
    be_expect_str_eq("/subdirectory/content/", otherStrPath.c_str())
    core::Path path(strPath);
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/", path.get_path().c_str())
    core::Path otherPath(otherStrPath);
    be_expect_str_eq("file:///subdirectory/content/", otherPath.get_url().c_str())

    path /= otherPath;
    be_expect_str_eq("file://", path.get_protocol().c_str())
    be_expect_str_eq("/", path.get_root().c_str())
    be_expect_str_eq("some/directory/", path.get_path().c_str())
    be_expect_eq(strlen("file:///some/directory/"), path.get_url().length())
    be_expect_str_eq("file:///some/directory/", path.get_url().c_str())
  }
  { // normalize
    {
      core::String strPath("/some/directory/./");

      be_expect_str_eq("/some/directory/./", strPath.c_str())
      core::Path path(strPath);
      be_expect_str_eq("some/directory/./", path.get_path().c_str())
      path.normalize();
      be_expect_str_eq("some/directory/", path.get_path().c_str())
    }
    {
      core::String strPath("/some/directory/../other_directory/");

      be_expect_str_eq("/some/directory/../other_directory/", strPath.c_str())
      core::Path path(strPath);
      be_expect_str_eq("some/directory/../other_directory/", path.get_path().c_str())
      path.normalize();
      be_expect_str_eq("some/other_directory/", path.get_path().c_str())
    }
    {
      core::String strPath("/some/directory/../../root/other_directory/");

      be_expect_str_eq("/some/directory/../../root/other_directory/", strPath.c_str())
      core::Path path(strPath);
      be_expect_str_eq("some/directory/../../root/other_directory/", path.get_path().c_str())
      path.normalize();
      be_expect_str_eq("root/other_directory/", path.get_path().c_str())
    }
    {
      core::String strPath("/some/directory/../../root/other_directory/");

      be_expect_str_eq("/some/directory/../../root/other_directory/", strPath.c_str())
      core::Path path(strPath);
      be_expect_str_eq("/some/directory/../../root/other_directory/", path.get_native_path().c_str())
      path.normalize();
      be_expect_str_eq("root/other_directory/", path.get_path().c_str())
      be_expect_str_eq("/root/other_directory/", path.get_native_path().c_str())
      be_expect_str_eq("/", path.get_root().c_str())
    }
  }

  // Easy appending via /-operator
  {
    {
      core::Path path("Example/");

      be_expect_str_eq("Example/", path.get_native_path().c_str());
      be_expect_true(path.is_directory());

      path /= "Blueprint/";
      be_expect_str_eq("Example/Blueprint/", path.get_native_path().c_str());
      be_expect_true(path.is_directory());

      path /= "Compositor/";
      be_expect_str_eq("Example/Blueprint/Compositor/", path.get_native_path().c_str());
      be_expect_true(path.is_directory());
    }

    {
      core::Path path("Example/");

      be_expect_str_eq("Example/", path.get_native_path().c_str());
      be_expect_true(path.is_directory());

      path = path / "Blueprint" / "Compositor";
      be_expect_str_eq("Example/Blueprint/Compositor/", path.get_path().c_str());
      be_expect_str_eq("Example/Blueprint/Compositor/", path.get_native_path().c_str());
      be_expect_true(path.is_directory());
    }
  }

  // For projects, we need "virtual" relative paths
  {
    {
      core::String strPath("Example/Blueprint/Compositor/");

      be_expect_str_eq("Example/Blueprint/Compositor/", strPath.c_str());
      core::Path path(strPath);
      be_expect_str_eq("Example/Blueprint/Compositor/", path.get_path().c_str());
      be_expect_str_eq("Example/Blueprint/Compositor/", path.get_native_path().c_str());
      be_expect_true(path.is_directory());
    }

    {
      core::String strPath("Example/Blueprint/Compositor/CN_Foobar.compositor_node");

      be_expect_str_eq("Example/Blueprint/Compositor/CN_Foobar.compositor_node", strPath.c_str());
      core::Path path(strPath);
      be_expect_str_eq("Example/Blueprint/Compositor/", path.get_path().c_str());
      be_expect_str_eq("Example/Blueprint/Compositor/CN_Foobar.compositor_node", path.get_native_path().c_str());
      be_expect_false(path.is_directory());
    }

    // Scheme test
    {
      core::String strPath("content:core/shader_pieces/sp_common.shader_piece");

      be_expect_str_eq(strPath.c_str(), core::Path(strPath).get_native_path().c_str());
    }
  }
}

be_unittest_autoregister(PathTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests