#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2025 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////

# /usr/lib/amdgpu-pro/:
##################################################
## Project
##################################################
re_add_target(
  NAME rhi_vulkan SHARED
  NAMESPACE RE
  FILES_CMAKE
    ${CMAKE_CURRENT_SOURCE_DIR}/rhi_vulkan_files.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/rhi_vulkan_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
  PLATFORM_INCLUDE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/rhi_vulkan_${PAL_PLATFORM_NAME_LOWERCASE}.cmake
  INCLUDE_DIRECTORIES
    PUBLIC
      ${RE_CONFIG_FILE_LOCATION}
      ${CMAKE_SOURCE_DIR}/.bitray_engine/vulkansdk-linux/1.3.239.0/x86_64/include
      #${CMAKE_SOURCE_DIR}/.bitray_engine/vulkansdk-linux/1.3.239.0/x86_64/include/vulkan
      #${CMAKE_SOURCE_DIR}/.bitray_engine/vulkansdk-linux/1.3.239.0/x86_64/include/glslang
      ${CMAKE_CURRENT_SOURCE_DIR}/public
      ${CMAKE_CURRENT_SOURCE_DIR}/private
      ${smolv_INCLUDE_DIR}
  BUILD_DEPENDENCIES
    PUBLIC
      core
      rhi
  RUNTIME_DEPENDENCIES
    PUBLIC
      core
      rhi
      External::smolv
  COMPILE_DEFINITIONS
    PUBLIC
      RHI_VULKAN_EXPORTS
      ${${PAL_PLATFORM_NAME_UPPERCASE}_COMPILE_DEFS}
      ${PAL_PLATFORM_NAME_UPPERCASE}
      # RHI_OPENGL_GLSLTOSPIRV
  TARGET_PROPERTIES
    -fPIC
)
