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
#include "core/platform/library_impl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class CORE_API LinuxLibrary : public LibraryImpl {
public:

  /**
   * @brief
   * Constructor.
   */
  LinuxLibrary();

  /**
   * @brief
   * Destructor.
   */
  ~LinuxLibrary();


//[-------------------------------------------------------]
//[ Protected LibraryImpl functions                        ]
//[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Returns if the dynamic library has been loaded
   *
   * @return
   * 'true' if loaded, else 'false'
   */
  [[nodiscard]] bool is_loaded() const override;

  /**
   * @brief
   * Loads a dynamic library
   *
   * @param[in] cUrl
   * Path to the dynamic library
   *
   * @return
   * 'true' if the library could be loaded, else false
   */
  [[nodiscard]] bool load() override;

  /**
   * @brief
   * get the absolute path to the dynamic library
   *
   * @return
   * The absolute path to the dynamic library (native path style)
   */
  [[nodiscard]] String get_abs_path() const override;

  /**
   * @brief
   * Unloads the dynamic library
   *
   * @return
   * 'true' if the library could be unloaded, else false
   */
  bool unload() override;

  /**
   * @brief
   * Returns a pointer to a symbol in the library
   *
   * @param[in] sSymbol
   * Name of the symbol to retrieve
   *
   * @return
   * Pointer to the symbol, or a null pointer on error
   *
   * @note
   * - The pointer to the symbol only stays valid as long as this dynamic library instance is not unloaded
   */
  [[nodiscard]] void *get_symbol(const String &sSymbol) const override;

private:
  /** Pointer to the loaded library */
  void *mLib;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core