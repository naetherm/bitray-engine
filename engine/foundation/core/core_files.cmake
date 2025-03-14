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

  # private/asset
  private/asset/asset.cpp
  private/asset/asset_package.cpp
  private/asset/asset_server.cpp
  private/asset/serializer/asset_package_serializer.cpp

  # private/color
  private/color/color3.cpp
  private/color/color4.cpp

  # private/config
  private/config/config.cpp
  private/config/config_reader.cpp
  private/config/config_server.cpp
  private/config/config_writer.cpp

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

  # private/input
  private/input/connection.cpp
  private/input/controller.cpp
  private/input/device_provider.cpp
  private/input/input_server.cpp
  private/input/control/axis.cpp
  private/input/control/button.cpp
  private/input/control/control.cpp
  private/input/device/connection_device.cpp
  private/input/device/device.cpp
  private/input/device/device_impl.cpp
  private/input/device/joystick.cpp
  private/input/device/keyboard.cpp
  private/input/device/mouse.cpp
  private/input/device/update_device.cpp
  private/input/virtual/virtual_controller.cpp
  private/input/virtual/virtual_standard_controller.cpp

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
  private/io/file_watcher.cpp
  private/io/file_watcher_impl.cpp
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
  private/math/frustum.cpp
  private/math/half.cpp
  private/math/line.cpp
  private/math/math.cpp
  private/math/plane.cpp
  private/math/ray.cpp
  private/math/transform.cpp

  # private/memory
  private/memory/linear_allocator.cpp
  private/memory/memory.cpp
  private/memory/memory_tracker.cpp
  private/memory/mimalloc_allocator.cpp
  private/memory/pool_allocator.cpp
  private/memory/stack_allocator.cpp
  private/memory/std_allocator.cpp

  # private/platform
  private/platform/condition_variable.cpp
  private/platform/console.cpp
  private/platform/library.cpp
  private/platform/library_impl.cpp
  private/platform/mutex.cpp
  private/platform/platform.cpp

  # private/plugin
  private/plugin/plugin.cpp
  private/plugin/plugin_server.cpp

  # private/resource
  private/resource/resource.cpp

  # private/rtti
  private/rtti/class.cpp
  private/rtti/enum.cpp
  private/rtti/module.cpp
  private/rtti/object.cpp
  private/rtti/rtti_member.cpp
  private/rtti/rtti_type_server.cpp
  private/rtti/enum/enum_value.cpp
  private/rtti/func/class_constructor.cpp
  private/rtti/func/class_field.cpp
  private/rtti/func/class_method.cpp
  private/rtti/func/class_property.cpp
  private/rtti/func/destructor.cpp
  private/rtti/func/function_signature.cpp
  private/rtti/tools/default_value.cpp
  private/rtti/tools/dynamic_object.cpp
  private/rtti/type_info/class_type_info.cpp
  private/rtti/type_info/enum_type_info.cpp
  private/rtti/type_info/pointer_type_info.cpp
  private/rtti/type_info/primitive_type_info.cpp
  private/rtti/type_info/reference_type_info.cpp
  private/rtti/type_info/type_info.cpp

  # private/std
  private/std/any.cpp

  # private/string
  private/string/string_hash.cpp

  # private/threading
  private/threading/runnable.cpp
  private/threading/thread.cpp
  private/threading/thread_impl.cpp

  # private/time
  private/time/stopwatch.cpp
  private/time/time_server.cpp

  # private/tools
  private/tools/command_line.cpp
  private/tools/command_line_option.cpp
  private/tools/compression_lz4.cpp
  private/tools/version.cpp

  # private/utility
  private/utility/fnv1a.cpp
)
