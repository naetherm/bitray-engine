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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/container/vector.h"
#include "core/container/hash_map.h"
#include "core/string/string.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct real_pcre;
typedef struct real_pcre pcre;
typedef struct pcre_extra pcre_extra;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Regular expression class (PCRE syntax)
*
*  @remarks
*    Internally the PCRE library (http://www.pcre.org) is used -
*    this here is only a wrapper interface.
*    This class supports ASCII and UTF8 encoding. You can force a certain encoding by setting
*    special flags like 'EncodingASCII' for forced ASCII encoding. If no such encoding flags
*    are set by the user, the internal string format of the given regular expression is used
*    as encoding.
*/
template<typename TCharType>
class BasicRegEx {


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Matching and encoding flags
  */
  enum EFlags {
    MatchCaseSensitive	= 1,	/** Match is case sensitive */
    MatchGreedy			= 2,	/** Match is greedy */
    EncodingASCII		= 4,	/** ASCII encoding, 1 byte per character (American Standard Code for Information Interchange, 0-128 defined, above undefined) */
    EncodingUTF8		= 8,	/** UTF8 encoding (8-bit Unicode Transformation Format) with 1 to 6 bytes to encode one Unicode character */
    Multiline			= 16,	/** PCRE-documentation says: "The "start of line" and "end of line" constructs match immediately following or immediately before internal newlines in the subject string, respectively, as well as at the very start and end" */
    DotAll				= 32	/** PCRE-documentation says: "A dot metacharacter in the pattern matches a character of any value, including one that indicates a newline" */
  };


  //[-------------------------------------------------------]
  //[ Public static functions                               ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Converts a given wildcard into a regular expression
  *
  *  @param[in] sWildcard
  *    String with wildcard to convert (for example "BeerNumber*")
  *
  *  @return
  *    The converted regular expression (for example "^BeerNumber.*$")
  */
  static CORE_API BasicString<TCharType> wildcard_to_regex(const BasicString<TCharType> &sWildcard);


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  inline BasicRegEx();

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] sExpression
  *    Regular expression
  *  @param[in] nFlags
  *    Processing mode and encoding (combination of EFlags values)
  */
  inline BasicRegEx(const BasicString<TCharType> &sExpression, uint32 nFlags = MatchCaseSensitive | MatchGreedy);

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cRegEx
  *    RegEx to copy
  */
  inline BasicRegEx(const BasicRegEx &cRegEx);

  /**
  *  @brief
  *    Destructor
  */
  CORE_API ~BasicRegEx();

  /**
  *  @brief
  *    Assignment operator
  *
  *  @param[in] cRegEx
  *    RegEx to copy
  *
  *  @return
  *    Reference to this instance
  */
  inline BasicRegEx &operator =(const BasicRegEx &cRegEx);

  /**
  *  @brief
  *    get regular expression
  *
  *  @return
  *    Regular expression
  */
  inline BasicString<TCharType> get_expression() const;

  /**
  *  @brief
  *    Set regular expression
  *
  *  @param[in] sExpression
  *    Regular expression
  *  @param[in] nFlags
  *    Processing mode  and encoding (combination of EFlags values)
  */
  CORE_API void set_expression(const BasicString<TCharType> &sExpression, uint32 nFlags = MatchCaseSensitive |
                                                                                            MatchGreedy);

  /**
  *  @brief
  *    Check if the regular expression is valid
  *
  *  @return
  *    Regular expression
  */
  inline bool is_valid() const;

  /**
  *  @brief
  *    get mode and encoding flags
  *
  *  @return
  *    Mode and encoding flags (combination of EFlags values)
  */
  inline uint32 get_flags() const;

  /**
  *  @brief
  *    Is the expression case sensitive?
  *
  *  @return
  *    'true', if the expression is treated case sensitive, else 'false'
  */
  inline bool is_case_sensitive() const;

  /**
  *  @brief
  *    Is the expression greedy?
  *
  *  @return
  *    'true', if the matching is greedy, else 'false'
  */
  inline bool is_greedy() const;

  /**
  *  @brief
  *    Is ASCII encoding used?
  *
  *  @return
  *    'true', if ASCII encoding, else 'false'
  */
  inline bool is_ascii() const;

  /**
  *  @brief
  *    Is UTF8 encoding used?
  *
  *  @return
  *    'true', if UTF8 encoding, else 'false'
  */
  inline bool is_utf8() const;

  /**
  *  @brief
  *    Analyze the regular expression and store additional internal information
  *
  *  @remarks
  *    This function analyses and stores additional internal information about the
  *    regular expression, which is later used to speed up processing. So you should
  *    call this e.g. for rather complicated expressions and/or expressions that
  *    are used many times.
  */
  CORE_API void study();

  /**
  *  @brief
  *    Check if a string matches the given regular expression
  *
  *  @param[in] sSubject
  *    String to test
  *  @param[in] nPosition
  *    start byte position in the string, for instance a value received by 'get_position()'
  *
  *  @remarks
  *    'sSubject' is internally automatically converted into the internal string format this
  *    regular expression is using. You can for instance use 'is_ascii()' to check whether or
  *    not ASCII is used.
  *
  *  @return
  *    'true' if the string matches, else 'false'
  */
  CORE_API bool match(const BasicString<TCharType> &sSubject, uint32 nPosition = 0);

  /**
  *  @brief
  *    get current byte position in the subject string
  *
  *  @return
  *    New byte position in the subject string after the last match
  */
  inline int get_position() const;

  /**
  *  @brief
  *    get the group results of the last check
  *
  *  @return
  *    List containing all matching groups
  */
  inline const Vector<BasicString<TCharType>> &get_results() const;

  /**
  *  @brief
  *    get a matching group from the last check
  *
  *  @param[in] nIndex
  *    Which group shall be returned?
  *
  *  @return
  *    The matching substring or ""
  */
  inline BasicString<TCharType> get_result(uint32 nIndex) const;

  /**
  *  @brief
  *    get the results of the last check by name
  *
  *  @return
  *    Map containing all matching groups as pairs of name->substring
  */
  inline const hash_map<BasicString<TCharType>, BasicString<TCharType>> &get_name_results() const;

  /**
  *  @brief
  *    get a matching group from the last check by name
  *
  *  @param[in] sName
  *    Which group shall be returned?
  *
  *  @return
  *    The matching substring or ""
  */
  inline BasicString<TCharType> get_name_result(const BasicString<TCharType> &sName) const;

  /**
  *  @brief
  *    get result (sub)string which was matched by the expression
  *
  *  @return
  *    The string which was matched by the expression
  */
  inline BasicString<TCharType> get_match_result() const;

  /**
  *  @brief
  *    Clear all results
  */
  inline void clear_results();


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Compiles the PCRE expression
  */
  void compile_pcre();

  /**
  *  @brief
  *    Deletes the PCRE expression
  */
  void free_pcre();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Regular expression
  BasicString<TCharType>					 m_sExpression;		/** Regular expression */
  pcre					*m_pPCRE;			/** PCRE expression, can be a null pointer */
  pcre_extra				*m_pExtra;			/** PCRE extra data, can be a null pointer */
  uint32					 m_nFlags;			/** Processing mode and encoding (combination of EFlags values) */

  // Matches
  Vector<BasicString<TCharType>>			 m_lstGroups;		/** List of substrings from the last match */
  hash_map<BasicString<TCharType>, BasicString<TCharType>>	 m_mapGroups;		/** Map of name->substrings pairs from the last match */
  int						 m_nPosition;		/** Current byte position in the subject */
  BasicString<TCharType>					 m_sMatchResult;	/** Contains the (sub)string which matches to the expression */
};

typedef BasicRegEx<char> CRegEx;
typedef BasicRegEx<wchar_t> WRegEx;
typedef BasicRegEx<char16_t> U16RegEx;
typedef BasicRegEx<char32_t> U32RegEx;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/string/regex.inl"