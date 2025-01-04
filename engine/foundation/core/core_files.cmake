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


set(FILES
  private/core.cpp

  # private/core
  private/core/default_assert.cpp
  private/core/refcounted.cpp

  # private/io
  private/io/file.cpp
  private/io/file_impl.cpp
  private/io/path.cpp

  # private/json
  private/json/json.cpp
  private/json/json_builder.cpp
  private/json/json_parser.cpp
  private/json/json_value.cpp
  private/json/json_writer.cpp

  # private/log
  private/log/log.cpp
  private/log/log_formatter.cpp
  private/log/log_formatter_text.cpp

  # private/memory
  private/memory/memory.cpp
  private/memory/mimalloc_allocator.cpp
  private/memory/std_allocator.cpp

  # private/platform
  private/platform/console.cpp
  private/platform/library.cpp
  private/platform/library_impl.cpp
  private/platform/platform.cpp

  # private/tools
  private/tools/command_line.cpp
  private/tools/command_line_option.cpp
  private/tools/compression_lz4.cpp
  private/tools/version.cpp
)
