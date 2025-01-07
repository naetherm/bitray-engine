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
#include "core/string/string.h"
#include "core/std/typetraits/integral_constant.h"


#if defined(DEBUG)
  #define STRING_HASH_SOURCE			   core::String mSourceString
  #define STRING_HASH_GET_SOURCE()	   const core::String& get_source_string() const { return mSourceString; }
  #define STRING_HASH_SET_SOURCE(SOURCE) mSourceString = SOURCE
#else
  #define STRING_HASH_SOURCE
	#define STRING_HASH_GET_SOURCE()
	#define STRING_HASH_SET_SOURCE(SOURCE)
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Compile time string ID class generating an identifier by hashing a given string
 *
 * @remarks
 * The following example shows how to use the string ID class:
 * @code
 * uint32 id = STRING_ID("Example/Mesh/Default/Squirrel");	// Result will be 2906231359
 * @endcode
 *
 * @note
 * - Is using compile-time string hashing as described within the Gamasutra article "In-Depth: Quasi Compile-Time String Hashing"
 *   by Stefan Reinalter ( http://www.gamasutra.com/view/news/38198/InDepth_Quasi_CompileTime_String_Hashing.php#.UG1MpVFQbq4 )
 * - As hash function FNV-1a is used ( http://isthe.com/chongo/tech/comp/fnv/ )
 */
class StringHash {


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  static constexpr uint32 FNV1a_INITIAL_HASH_32 = 0xcbf29ce4u;
  static constexpr uint32 FNV1a_MAGIC_PRIME_32 = 0x1000193u;
  static constexpr uint32 MAXIMUM_UINT32_T_VALUE = 4294967295u;  ///< We don't want to include "<limits>" in this lightweight core header just to be able to use "std::numeric_limits<uint32>::max()"

  /**
   * @brief
   * "const char*"-wrapper enabling the compiler to distinguish between static and dynamic strings
   */
  struct ConstCharWrapper {
    /**
     * @brief
     * "const char*"-wrapper constructor
     *
     * @param[in] string
     * Null terminated ASCII string to calculate the hash value for, must be valid
     *
     * @note
     * - Not explicit by intent
     */
    ConstCharWrapper(const char *string);

    const char *mString;  ///< Null terminated ASCII string to calculate the hash value for, must be valid
  };


  //[-------------------------------------------------------]
  //[ Public static methods                                 ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Calculate the hash value of the given string at compile time
   * 
   * @param[in] string
   * Null terminated ASCII string to calculate the hash value for, must be valid
   * @param[in] value
   * Initial hash
   * 
   * @return
   * The hash value of the given string
   */
  [[nodiscard]] static constexpr inline uint32 compile_time_fnv(const char *string, const uint32 value = FNV1a_INITIAL_HASH_32) noexcept;

  /**
   * @brief
   * Calculate the hash value of the given string
   * 
   * @param[in] string
   * Null terminated ASCII string to calculate the hash value for, must be valid
   * 
   * @return
   * The hash value of the given string
   */
  [[nodiscard]] static uint32 calculate_fnv(const char *string);


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Default constructor
   */
  StringHash();

  /**
   * @brief
   * Constructor for calculating the hash value of a given static string
   * 
   * @param[in] string
   * Static string to calculate the hash value for, must be valid
   */
  template<uint32 N>
  StringHash(const char (&string)[N]) noexcept;

  /**
   * @brief
   * Constructor for calculating the hash value of a given dynamic string
   * 
   * @param[in] string
   * Dynamic string to calculate the hash value for, must be valid
   */
  explicit StringHash(const ConstCharWrapper &string);

  /**
   * @brief
   * Constructor for directly setting an uint32 value as string ID
   * 
   * @param[in] id
   * ID value to set
   * 
   * @note
   * - By intent not explicit for best possible usability
   */
  StringHash(uint32 id);

  /**
   * @brief
   * Copy constructor
   * 
   * @param[in] stringId
   * String ID instance to copy from
   * 
   * @remarks
   * This constructor is by intent not explicit because otherwise GCC 4.9 throws an error when "Renderer::StringHash" is used
   * e.g. as a function parameter and the function is called with a string literal. Example:
   * "
   * typedef StringHash AssetId;
   * void function(StringHash assetId) {}
   * void functionAssetId(AssetId assetId) {}
   * "
   * Function call results:
   * - function("BlaBlub"); <-- GCC 4.9 error: no matching function for call to "Renderer::StringHash::StringHash(Renderer::StringHash)"
   * - functionAssetId("BlaBlub"); <-- GCC 4.9 error: no matching function for call to "Renderer::StringHash::StringHash(AssetId)"
   */
  StringHash(const StringHash &stringId);

  /**
   * @brief
   * Return the generated ID
   * 
   * @return
   * The generated FNV-1a hash value used as identifier
   */
  [[nodiscard]] uint32 get_id() const;

  /**
   * @brief
   * Return the generated ID
   * 
   * @return
   * The generated FNV-1a hash value used as identifier
   */
  [[nodiscard]] operator uint32() const;

  STRING_HASH_GET_SOURCE()


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** The generated FNV-1a hash value which is used as identifier */
  uint32 mId;
  STRING_HASH_SOURCE;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/string/string_hash.inl"


//[-------------------------------------------------------]
//[ Macros & definitions                                  ]
//[-------------------------------------------------------]
#ifdef _MSC_VER
// Disable warnings in external headers, we can't fix them: warning C4307: '*': integral constant overflow
/**
 * @brief
 * Compile time string ID macro
 */
#define STRING_HASH(string) \
    __pragma(warning(push)) \
      __pragma(warning(disable:4307)) \
      std::integral_constant<core::uint32, core::StringHash::compile_time_fnv(string)>::value \
    __pragma(warning(pop))
#else
/**
 * @brief
 * Compile time string ID macro
 */
#define STRING_HASH(string) core::integral_constant<core::uint32, core::StringHash::compile_time_fnv(string)>::value
#endif