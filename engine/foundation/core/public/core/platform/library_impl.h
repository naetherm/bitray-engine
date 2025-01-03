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
#include "core/io/path.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Abstract base class for platform specific 'Library' implementations
 *
 * @note
 * - Implementation of the bridge design pattern, this class is the implementor of the 'Library' abstraction
 */
class LibraryImpl {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class Library;


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Constructor
  */
  LibraryImpl() = default;


  //[-------------------------------------------------------]
  //[ Protected virtual LibraryImpl functions               ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Destructor
   */
  virtual ~LibraryImpl() = default;


  /**
   * @brief
   * Sets the path of the library.
   *
   * @param[in] path
   * The path of the library.
   */
  void set_path(const Path& path);

  /**
   * @brief
   * The path of the library.
   *
   * @return
   * The path of the library.
   */
  [[nodiscard]] const Path& get_path() const;

protected:
  /**
   * @brief
   * Returns if the dynamic library has been loaded
   *
   * @return
   * 'true' if loaded, else 'false'
   */
  [[nodiscard]] virtual bool is_loaded() const = 0;

  /**
   * @brief
   * Loads a dynamic library
   *
   * @return
   * 'true' if the library could be loaded, else false
   */
  [[nodiscard]] virtual bool load() = 0;

  /**
   * @brief
   * get the absolute path to the dynamic library
   *
   * @return
   * The absolute path to the dynamic library (native path style)
   */
  [[nodiscard]] virtual String get_abs_path() const = 0;

  /**
   * @brief
   * Unloads the dynamic library
   *
   * @return
   * 'true' if the library could be unloaded, else false
   */
  virtual bool unload() = 0;

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
  [[nodiscard]] virtual void *get_symbol(const String &sSymbol) const = 0;

protected:
  /** Path to the library */
  Path mLibraryPath;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core