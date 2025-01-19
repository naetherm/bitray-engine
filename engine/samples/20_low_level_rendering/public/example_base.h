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
#include <core/log/log.h>
#include <core/string/string.h>
#include <rhi/rhi_headers.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Application;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * ExampleBase
 *
 * @brief
 * Example base class
 */
class ExampleBase {
public:

  /**
   * @brief
   * Constructor.
   */
  inline ExampleBase(Application& cApplication, const core::String& name)
  : mApplication(&cApplication)
  , mExampleName(name) {

  }

  /**
   * @brief
   * Destructor.
   */
  inline virtual ~ExampleBase() {

  }

  [[nodiscard]] const core::String& get_example_name() const {
    return mExampleName;
  }

  inline void draw(rhi::RHICommandBuffer& commandBuffer) {
    this->on_draw(commandBuffer);
  }

  [[nodiscard]] rhi::RHIDevice* get_rhi() const;

  [[nodiscard]]  rhi::RHIRenderTarget* get_main_render_target() const;

public:

  inline virtual void on_initialization() {

  }

  inline virtual void on_deinitialization() {

  }

  inline virtual void on_update() {

  }

  inline virtual void on_draw(rhi::RHICommandBuffer& commandBuffer) {

  }

  /**
   * @brief
   * Return if the example does the whole drawing process by itself.
   *
   * @return
   * True if example does its complete drawing handling, false otherwise.
   */
  [[nodiscard]] inline virtual bool does_complete_own_drawing() const {
    return false;
  }

private:
  /** Pointer to the application instance */
  Application* mApplication;
  /** The name of the current example */
  core::String mExampleName;
};