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
#include "core/core/refcounted.h"
#include "core/string/string.h"
#include "core/io/path.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class LibraryImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Dynamic library (aka "shared library") class
 *
 * @note
 * - Implementation of the bridge design pattern, this class is the abstraction
 */
class CORE_API Library : public RefCounted {
  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   */
  Library();

  /**
   * @brief
   * Destructor
   *
   * @note
   * - The dynamic library is unloaded automatically
   */
  ~Library();


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
   * Loads a dynamic library
   *
   * @return
   * 'true' if the library could be loaded, else 'false'
   *
   * @note
   * - Dependent dynamic libraries are first searched within the same path as the given one
   * - If the library has already been loaded, this function will fail
   */
  [[nodiscard]] inline bool load();

  /**
   * @brief
   * Returns if the dynamic library has been loaded
   *
   * @return
   * 'true' if loaded, else 'false'
   */
  [[nodiscard]] inline bool is_loaded() const;

  /**
   * @brief
   * get the path (set within 'Load()') to the dynamic library
   *
   * @return
   * The path to the dynamic library (e.g. "file://C:/"BitrayEngine/Runtime/x86/PLCore.dll" on Windows)
   */
  [[nodiscard]] inline const Path& get_path() const;

  /**
   * @brief
   * get the absolute path to the dynamic library
   *
   * @return
   * The absolute path to the dynamic library (native path style, e.g. "C:\BitrayEngine\Runtime\x86\PLCore.dll" on Windows)
   */
  [[nodiscard]] inline String get_abs_path() const;

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
  [[nodiscard]] inline void *get_symbol(const String &sSymbol) const;

  /**
   * @brief
   * Unloads the dynamic library
   *
   * @return
   * 'true' if the library could be unloaded, else false
   */
  [[nodiscard]] inline bool unload();


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Copy constructor
   *
   * @param[in] cSource
   * Source to copy from
   */
  Library(const Library &cSource);

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
  Library &operator=(const Library &cSource);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Pointer to the system specific implementation (assumed to be never a null pointer!) */
  LibraryImpl *mImpl;
  /** String to the dynamic library */
  String mUrl;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/platform/library.inl"