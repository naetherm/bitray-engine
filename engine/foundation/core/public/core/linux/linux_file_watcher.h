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
#include "core/io/file_watcher_impl.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * LinuxFileWatcher
 *
 * @brief
 * Filesystem watcher implementation for linux.
 */
class LinuxFileWatcher : public FileWatcherImpl {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] path
   * The path to watch.
   */
  explicit LinuxFileWatcher(const core::Path& path);

  /**
   * @brief
   * Constructor.
   *
   * @param[in] watcherPath
   * The path to watch.
   */
  explicit LinuxFileWatcher(const core::String& watcherPath);

  /**
   * @brief
   * Destructor.
   */
  ~LinuxFileWatcher() override;


  /**
   * @brief
   * Returns if there are any changes to the observed filesystem path.
   *
   * @return
   * 'true' if there are any changes noted, 'false' otherwise.
   */
  bool any_changes() const override;

  /**
   * @brief
   * Closes the watching process.
   */
  void close() override;

private:
  /**
   * @brief
   * Initializes the notification handle.
   */
  void initialize_notification_handle();

private:
  /** Handle to notification item */
  core::int32 mHandle;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}