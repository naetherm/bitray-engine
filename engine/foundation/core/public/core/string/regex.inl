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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <pcre.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// Maximum number of arguments we can set
static const int MaxGroups	= 11;
static const int VecSize	= (MaxGroups+1) * 3;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::wildcard_to_regex(const BasicString<TCharType> &sWildcard) {
  BasicString<TCharType> sString = '^';

  // Loop through all characters
  for (uint32 i=0, nLength=sWildcard.length(); i<nLength; i++) {
    const char szCharacter = sWildcard[i];
    switch (szCharacter) {
      case '*':
        sString += ".*";
        break;

      case '?':
        sString += '.';
        break;

        // Escape special regex-characters
      case '(': case ')': case '[': case ']': case '$':
      case '^': case '.': case '{': case '}': case '|':
      case '\\':
        sString += '\\';
        sString += szCharacter;
        break;

      default:
        sString += szCharacter;
        break;
    }
  }

  // End
  sString += '$';

  // Return the regular expression
  return sString;
}

template<typename TCharType>
BasicRegEx<TCharType>::BasicRegEx() :
m_pPCRE(nullptr),
m_pExtra(nullptr),
m_nFlags(MatchCaseSensitive | MatchGreedy),
m_nPosition(0) {

}

template<typename TCharType>
BasicRegEx<TCharType>::BasicRegEx(const BasicString<TCharType> &sExpression, uint32 nFlags) :
m_pPCRE(nullptr),
m_pExtra(nullptr),
m_nFlags(nFlags),
m_nPosition(0) {
  // Set expression
  set_expression(sExpression, nFlags);
}

template<typename TCharType>
BasicRegEx<TCharType>::BasicRegEx(const BasicRegEx &cRegEx) :
m_pPCRE(nullptr),
m_pExtra(nullptr),
m_nFlags(cRegEx.m_nFlags),
m_nPosition(0) {
  // Copy expression
  set_expression(cRegEx.m_sExpression, cRegEx.m_nFlags);
}

template<typename TCharType>
BasicRegEx<TCharType>::~BasicRegEx() {
  // release expression
  free_pcre();
}

template<typename TCharType>
BasicRegEx<TCharType> &BasicRegEx<TCharType>::operator=(const BasicRegEx &cRegEx) {
  // Copy expression
  set_expression(cRegEx.m_sExpression, cRegEx.m_nFlags);

  // Return this instance
  return *this;
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::get_expression() const {
  // Return expression
  return m_sExpression;
}

template<typename TCharType>
void BasicRegEx<TCharType>::set_expression(const BasicString<TCharType> &sExpression, uint32 nFlags) {
  // Save expression and options
  m_sExpression	= sExpression;
  m_nFlags		= nFlags;

  // Any encoding flags set by the user?
  const bool bEncodingASCII = (m_nFlags & EncodingASCII) != 0;
  const bool bEncodingUTF8  = (m_nFlags & EncodingUTF8)  != 0;

  // ASCII and UTF8 flags set at the same time?
  if (bEncodingASCII && bEncodingUTF8)
    m_nFlags |= ~EncodingASCII; // Remove the ASCII flag!

    // get the encoding automatically?
  else if (!bEncodingASCII && !bEncodingUTF8)
    m_nFlags |= EncodingASCII;

  // Compile expression
  compile_pcre();
}

template<typename TCharType>
bool BasicRegEx<TCharType>::is_valid() const {
  // Return valid flag
  return (m_pPCRE != nullptr);
}

template<typename TCharType>
uint32 BasicRegEx<TCharType>::get_flags() const {
  return m_nFlags;
}

template<typename TCharType>
bool BasicRegEx<TCharType>::is_case_sensitive() const {
  return ((m_nFlags & MatchCaseSensitive) != 0);
}

template<typename TCharType>
bool BasicRegEx<TCharType>::is_greedy() const {
  return ((m_nFlags & MatchGreedy) != 0);
}

template<typename TCharType>
bool BasicRegEx<TCharType>::is_ascii() const {
  return ((m_nFlags & EncodingASCII) != 0);
}

template<typename TCharType>
bool BasicRegEx<TCharType>::is_utf8() const {
  return ((m_nFlags & EncodingUTF8) != 0);
}

template<typename TCharType>
void BasicRegEx<TCharType>::study() {
  if (m_pPCRE) {
    const char *pszError;
    m_pExtra = pcre_study(m_pPCRE, 0, &pszError);
  }
}

template<typename TCharType>
bool BasicRegEx<TCharType>::match(const BasicString<TCharType> &sSubject, uint32 nPosition) {
  if (m_pPCRE) {
    // Clear results of last operation
    clear_results();

    // Early escape test: In case we try matching when we are already at the end of the string, we know the result without doing any matches
    if (nPosition < sSubject.length()) {
      // Match pattern
      int nMatches[VecSize];
      int nMatchCount = pcre_exec(
        m_pPCRE,								// PCRE expression
        m_pExtra,								// Extra data
        sSubject.data(),					// Subject string
        sSubject.size(),	// Subject length in bytes
        nPosition,								// start at byte position
        0,										// Default options
        nMatches,								// Output vector with offsets in bytes
        VecSize									// Size of output vector
      );

      // Output vector too small?
      if (nMatchCount == 0) {
        // Allocate big enough output vector and retry
        // [TODO]
        nMatchCount = MaxGroups;
      }

      // Check result
      if (nMatchCount > 0) {
        // Save new byte position within the string
        m_nPosition = nMatches[1];

        // get pointer to subject string
        const char *pszSubject = sSubject.c_str();
        // The first two elements in nMatches describe the result string which was matched by the expression
        // e.g. source string is: "Hallo World!", the expression is "Hal+o" -> first element = "Hallo"
        //m_sMatchResult.copy(&pszSubject[nMatches[0]], nMatches[1] - nMatches[0]);
        m_sMatchResult = sSubject.substr(nMatches[0], nMatches[1] - nMatches[0]);

        // Save matching substrings which have a group description in the expression by index
        m_lstGroups.resize(nMatchCount-1);
        for (int i=1; i<nMatchCount; i++) {
          //m_lstGroups[i-1].copy(&pszSubject[nMatches[i*2]], nMatches[i*2+1] - nMatches[i*2]);
          m_lstGroups[i-1] = sSubject.substr(nMatches[i*2], nMatches[i*2+1] - nMatches[i*2]);
        }
        // get named groups
        int nNameCount;
        pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMECOUNT, &nNameCount);
        const char *pszNameTable;
        pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMETABLE, &pszNameTable);
        int nNameEntrySize;
        pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMEENTRYSIZE, &nNameEntrySize);

        // Return matching substrings by name
        for (int i=0; i<nNameCount; i++) {
          // get pointer to current entry
          const char *pszEntry = &pszNameTable[i*nNameEntrySize];

          // get number
          const int nNum = (pszEntry[0] << 8) | pszEntry[1];

          // get name
          const BasicString<TCharType> sName = &pszEntry[2];

          // get substring
          const int nIndex0 = nMatches[nNum*2];
          const int nIndex1 = nMatches[nNum*2+1];

          // Add name->substring to map
          if (nIndex0 >= 0 && nIndex1 >= 0)
            m_mapGroups.emplace(sName, String(&pszSubject[nIndex0], true, nIndex1 - nIndex0));
          else
            m_mapGroups.emplace(sName, "");
        }

        // Done
        return true;
      }
    }
  }

  // Error!
  return false;
}

template<typename TCharType>
int BasicRegEx<TCharType>::get_position() const {
  return m_nPosition;
}

template<typename TCharType>
const Vector<BasicString<TCharType>> &BasicRegEx<TCharType>::get_results() const {
  return this->m_lstGroups;
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::get_result(uint32 nIndex) const {
  return m_lstGroups[nIndex];
}

template<typename TCharType>
const hash_map<BasicString<TCharType>, BasicString<TCharType>> &BasicRegEx<TCharType>::get_name_results() const {
  return this->m_mapGroups;
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::get_name_result(const BasicString<TCharType> &sName) const {
  return m_mapGroups.Get(sName);
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::get_match_result() const {
  return m_sMatchResult;
}

template<typename TCharType>
void BasicRegEx<TCharType>::clear_results() {
  // Clear result string
  m_sMatchResult = "";

  // Clear lists for groups
  m_lstGroups.clear();
  m_mapGroups.clear();

  // reset byte position
  m_nPosition = 0;
}

template<typename TCharType>
void BasicRegEx<TCharType>::compile_pcre() {
  // Free former expression
  free_pcre();

  // Set options
  int nOptions = (is_case_sensitive() ? 0 : PCRE_CASELESS) | (is_greedy() ? 0 : PCRE_UNGREEDY);
  if (m_nFlags & Multiline)
    nOptions |= PCRE_MULTILINE;
  if (m_nFlags & DotAll)
    nOptions |= PCRE_DOTALL;

  const char *pszError;
  int nErrorOffset;

  // Compile regular expression
  m_pPCRE = pcre_compile(m_sExpression.c_str(), nOptions, &pszError, &nErrorOffset, nullptr);


  if (!m_pPCRE) {
    // Error!
  }
}

template<typename TCharType>
void BasicRegEx<TCharType>::free_pcre() {
  // Free PCRE expression
  if (m_pPCRE) {
    pcre_free(m_pPCRE);
    m_pPCRE  = nullptr;
    m_pExtra = nullptr;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]