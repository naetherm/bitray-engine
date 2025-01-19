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
#include <core/frontend/frontend_application.h>
#include <core/platform/library.h>
#include <rhi/rhi_headers.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ExampleBase;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Application
 *
 * @brief
 * Low level rendering testing application implementation.
 */
class Application : public core::FrontendApplication {

  friend class ExampleBase;

public:

  /**
   * @brief
   * Constructor.
   *
   * @param frontend
   */
  Application();

  /**
   * @brief
   * Destructor
   */
  ~Application() override;


protected:
  /**
   * @brief
   * Initialization function that is called prior to OnInit()
   *
   * @return
   * 'true' if all went fine, else 'false' which will stop the application
   *
   * @remarks
   * The default implementation does the following tasks:
   * - Everything that PLCore::CoreApplication::OnStart() does
   * - Call OnCreateRPIContext()
   * - Call OnCreatePainter()
   * - Return and go on with OnInit()
   */
  bool on_start() override;

  /**
   * @brief
   * De-initialization function that is called after OnDeInit()
   *
   * @remarks
   * The default implementation does the following tasks:
   * - Save renderer related configuration
   * - Destroy renderer context
   * - Everything that FrontendApplication::OnStop() does
   */
  void on_stop() override;

  /**
   * @brief
   * Called when the window size has been changed
   *
   * @note
   * - The default implementation is empty
   */
  void on_size() override;

  /**
   * @brief
   * Called to let the frontend update it's states
   *
   * @remarks
   * The default implementation does the following tasks:
   * - Everything that PLCore::FrontendApplication::OnUpdate() does
   * - Update renderer context
   */
  void on_update() override;


  void on_draw() override;

private:

  void main() override;

protected:
  /**
   * @brief
   * Function that is called to Create the application's renderer context
   *
   * @note
   * - Part of the application framework initialization function "on_start()"
   */
  virtual void on_create_renderer_context();

private:
  /** Pointer to the dynamic library, the dynamic rhi was loaded from. This is always valid! */
  core::Library  mRhiSharedLibrary;
  /** The name of the library to load */
  core::String msLibraryName;
  /** Pointer to rhi context */
  rhi::RHIContext *mpRHIContext;
  /** RHI instance, can be a null pointer, do not destroy the instance */
  rhi::RHIDevice*		   mRhi;
  /** Main swap chain instance, can be a null pointer, release the instance if you no longer need it */
  rhi::RHISwapChain*   mMainSwapChain;
  /** Command buffer */
  rhi::RHICommandBuffer mCommandBuffer;

  ExampleBase* mExampleBase;
};