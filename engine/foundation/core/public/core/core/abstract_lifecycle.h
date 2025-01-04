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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * AbstractLifecycle
 *
 * @brief
 * Abstract life cycle class
 *
 * @remarks
 * @verbatim
 *    The "Life cycle" is:
 *      "on_create()"         - Called directly after the object has been created
 *        ("on_restart()")    - Called directly before a stopped object is going to start again (always followed by "on_start()")
 *        "on_start()"        - Called when the object becoming visible to the user
 *          "on_resume()"     - Called when the object has the focus (keep the implementation lightweight)
 *          "on_pause()"      - Called when the object has no longer the focus (keep the implementation lightweight)
 *        "on_stop()"         - Called when the object is no longer visible to the user
 *      "on_destroy()"        - Called before the object is going to be finally destroyed
 *    This life cycle has the same interface as the "Android Activity Life cycle"
 *    (http://developer.android.com/reference/android/app/Activity.html#ActivityLifecycle)
 *    because this interface looks just the right way to go for this purpose... also there
 *    are nice diagrams and documentations explaining the interface so it shouldn't be too
 *    hard to understand it.
 * @endverbatim
 */
class AbstractLifecycle {


  //[-------------------------------------------------------]
  //[ Public virtual AbstractLifecycle functions            ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Called directly after the object has been created
   */
  virtual void on_create() = 0;

  /**
   * @brief
   * Called directly before a stopped object is going to start again (always followed by "on_start()")
   */
  virtual void on_restart() = 0;

  /**
   * @brief
   * Called when the object becoming visible to the user
   *
   * @return
   * 'true' if all went fine, else 'false' (on failure, no "on_resume()", "on_pause()" or "on_stop()" will be called)
   */
  [[nodiscard]] virtual bool on_start() = 0;

  /**
   * @brief
   * Called when the object has the focus (keep the implementation lightweight)
   */
  virtual void on_resume() = 0;

  /**
   * @brief
   * Called when the object has no longer the focus (keep the implementation lightweight)
   */
  virtual void on_pause() = 0;

  /**
   * @brief
   * Called when the object is no longer visible to the user
   */
  virtual void on_stop() = 0;

  /**
   * @brief
   * Called before the object is going to be finally destroyed
   */
  virtual void on_destroy() = 0;


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Default constructor
   */
  AbstractLifecycle() = default;

  /**
   * @brief
   * Destructor
   */
  virtual ~AbstractLifecycle() = default;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
   * @brief
   * Copy constructor
   *
   * @param[in] cSource
   * Source to copy from
   */
  AbstractLifecycle(const AbstractLifecycle &cSource) = default;

  /**
   * @brief
   * Copy operator
   *
   * @param[in] cSource
   * Source to copy from
   *
   * @return
   * Reference to this instance
   */
  AbstractLifecycle &operator=(const AbstractLifecycle &cSource) = default;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core