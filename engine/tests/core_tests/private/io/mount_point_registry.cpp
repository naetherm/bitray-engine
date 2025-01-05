////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2023 RacoonStudios
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
#include "io/mount_point_registry.h"
#include <core/io/io_server.h>
#include <core/io/mount_point_registry.h>
#include <core/io/types.h>
#include <core/log/log.h>
#include <core/platform/platform.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core_tests {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
MountPointRegistryTests::MountPointRegistryTests()
  : UnitTest("MountPointRegistryTests") {

}

MountPointRegistryTests::~MountPointRegistryTests() {

}

void MountPointRegistryTests::test() {
  core::String currentDir = core::Platform::instance().get_current_dir();
  core::String mountPoint = "project";

  core::MountPointRegistry mpr;

  {
    mpr.add_mount_point(mountPoint, currentDir);

    be_expect_true(mpr.has_mount_point(mountPoint));
    be_expect_str_eq(currentDir.c_str(), mpr.get_real_path_of_mount(mountPoint).c_str());
  }

  {
    core::String dependentDir = "project:content";
    core::String secondMountPoint = "content";
    mpr.add_mount_point(secondMountPoint, dependentDir);
    be_expect_true(mpr.has_mount_point(mountPoint));
    be_expect_str_eq(currentDir.c_str(), mpr.get_real_path_of_mount(mountPoint).c_str());
    be_expect_true(mpr.has_mount_point(secondMountPoint));
    be_expect_str_eq(dependentDir.c_str(), mpr.get_real_path_of_mount(secondMountPoint).c_str());
  }

  // Interpret and resolve paths
  {
    core::String stringPath = "content:core/shader_pieces/sp_common.shader_piece";
    core::String wrongPath = "http://some_space.com/foobar";
    be_expect_str_eq(
      (currentDir + "/content/core/shader_pieces/sp_common.shader_piece").c_str(),
      mpr.resolve_path(stringPath).c_str());
    be_expect_str_eq(
      wrongPath.c_str(),
      mpr.resolve_path(wrongPath).c_str());

  }
  {
    core::String stringPath = "content:core/shader_pieces/sp_common.shader_piece";
    core::String wrongPath = "http://some_space.com/foobar";
    be_expect_str_eq(
      (currentDir + "/content/core/shader_pieces/sp_common.shader_piece").c_str(),
      mpr.resolve_path(core::Path(stringPath)).get_native_path().c_str());
    be_expect_str_eq(
      wrongPath.c_str(),
      mpr.resolve_path(core::Path(wrongPath)).get_native_path().c_str());

  }
}

be_unittest_autoregister(MountPointRegistryTests)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core_tests