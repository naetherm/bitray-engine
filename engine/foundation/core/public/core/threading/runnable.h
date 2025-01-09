////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Runnable
 *
 * @brief
 * Runnable implementation, which are used within the multi-threading pipeline.
 */
class Runnable {

  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:

  /**
   * @brief
   * Constructor.
   */
  Runnable() = default;

  /**
   * @brief
   * Destructor.
   */
  virtual ~Runnable() = default;


  /**
   * @brief
   * Called on initialization.
   *
   * @return
   */
  virtual bool init();

  /**
   * @brief
   * This method is called on stop.
   */
  virtual void stop();

  /**
   * @brief
   * This method is called on exit.
   */
  virtual void exit();

  /**
   * @brief
   * This method is called when the threads reaches the end of the runnable instance.
   *
   * @param[in] killed
   * True if the thread itself was killed.
   */
  virtual void post_work(bool killed);


  /**
   * @brief
   * Runs the runnable instance.
   *
   * @return
   * The return code of the runnable.
   */
  virtual int32 run() = 0;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core