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


##################################################
## Project
##################################################
re_add_target(
  NAME game_toolkit SHARED
  NAMESPACE RE
  FILES_CMAKE
    ${CMAKE_CURRENT_SOURCE_DIR}/game_toolkit_files.cmake
  INCLUDE_DIRECTORIES
    PUBLIC
      ${RE_CONFIG_FILE_LOCATION}
      ${CMAKE_CURRENT_SOURCE_DIR}/public
      ${CMAKE_CURRENT_SOURCE_DIR}/private
  BUILD_DEPENDENCIES
    PUBLIC
      core
  RUNTIME_DEPENDENCIES
    PUBLIC
      core
  COMPILE_DEFINITIONS
    PUBLIC
      GAME_TOOLKIT_EXPORTS
      ${${PAL_PLATFORM_NAME_UPPERCASE}_COMPILE_DEFS}
      ${PAL_PLATFORM_NAME_UPPERCASE}
  TARGET_PROPERTIES
    -fPIC
)