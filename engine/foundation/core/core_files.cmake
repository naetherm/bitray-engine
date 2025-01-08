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

  # private/app
  private/app/core_application.cpp

  # private/core
  private/core/default_assert.cpp
  private/core/refcounted.cpp

  # private/frontend
  private/frontend/cursor.cpp
  private/frontend/cursor_impl.cpp
  private/frontend/cursor_device.cpp
  private/frontend/display_device.cpp
  private/frontend/frontend_application.cpp
  private/frontend/frontend_server.cpp
  private/frontend/window.cpp
  private/frontend/window_impl.cpp
  private/frontend/window_device.cpp

  # private/io
  private/io/binary_reader.cpp
  private/io/binary_writer.cpp
  private/io/compressed_file.cpp
  private/io/compressed_reader.cpp
  private/io/compressed_stream.cpp
  private/io/compressed_writer.cpp
  private/io/file.cpp
  private/io/file_impl.cpp
  private/io/file_reader.cpp
  private/io/file_stream.cpp
  private/io/file_writer.cpp
  private/io/filesystem.cpp
  private/io/filesystem_helper.cpp
  private/io/io_server.cpp
  private/io/json_reader.cpp
  private/io/json_writer.cpp
  private/io/media_type.cpp
  private/io/memory_stream.cpp
  private/io/mount_point.cpp
  private/io/mount_point_registry.cpp
  private/io/path.cpp
  private/io/stream.cpp
  private/io/stream_reader.cpp
  private/io/stream_writer.cpp
  private/io/text_reader.cpp
  private/io/text_writer.cpp

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

  # private/math
  private/math/coordinate_system.cpp
  private/math/line.cpp
  private/math/math.cpp
  private/math/ray.cpp

  # private/memory
  private/memory/memory.cpp
  private/memory/mimalloc_allocator.cpp
  private/memory/std_allocator.cpp

  # private/platform
  private/platform/condition_variable.cpp
  private/platform/console.cpp
  private/platform/library.cpp
  private/platform/library_impl.cpp
  private/platform/mutex.cpp
  private/platform/platform.cpp

  # private/string
  private/string/string_hash.cpp

  # private/tools
  private/tools/command_line.cpp
  private/tools/command_line_option.cpp
  private/tools/compression_lz4.cpp
  private/tools/version.cpp

  # private/utility
  private/utility/fnv1a.cpp
)
