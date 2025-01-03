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
#include "core/string/buffered_reader_string.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
template<typename TCharType>
const BasicString<TCharType> BasicTokenizer<TCharType>::EndOfLine = '\n';


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TCharType>
BasicTokenizer<TCharType>::BasicTokenizer():
m_sDelimiters(" \t\r\n"),
m_sSingleChars("{}()[]<*>/=,;\""),
m_sQuotes("\"\'"),
m_sCommentStartTag("/*"),
m_sCommentEndTag("*/"),
m_sSingleLineComment("//"),
m_bCaseSensitive(false),
m_pStream(nullptr),
m_nPosition(0),
m_nLine(0),
m_nParseMode(eEatGarbage) {

}

template<typename TCharType>
BasicTokenizer<TCharType>::~BasicTokenizer() {
  // stop tokenizer
  stop();
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_delimiters() const {
  return m_sDelimiters;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::set_delimiters(const BasicString<TCharType> &sDelimiters) {
  m_sDelimiters = sDelimiters;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_single_chars() const {
  return m_sSingleChars;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::set_single_chars(const BasicString<TCharType> &sSingleChars) {
  m_sSingleChars = sSingleChars;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_quotes() const {
  return m_sQuotes;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::set_quotes(const BasicString<TCharType> &sQuotes) {
  m_sQuotes = sQuotes;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_comment_start_tag() const {
  return m_sCommentStartTag;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::set_comment_start_tag(const BasicString<TCharType> &sCommentStartTag) {
  m_sCommentStartTag = sCommentStartTag;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_comment_end_tag() const {
  return m_sCommentEndTag;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::set_comment_end_tag(const BasicString<TCharType> &sCommentEndTag) {
  m_sCommentEndTag = sCommentEndTag;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_single_line_comment() const {
  return m_sSingleLineComment;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::set_single_line_comment(const BasicString<TCharType> &sSingleLineComment) {
  m_sSingleLineComment = sSingleLineComment;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::is_case_sensitive() const {
  return m_bCaseSensitive;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::set_case_sensitive(bool bCaseSensitive) {
  m_bCaseSensitive = bCaseSensitive;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::start(const BasicString<TCharType> &sBuffer) {
  // start tokenizer
  stop();
  m_pStream = new BasicBufferedReaderString<TCharType>(sBuffer);
}
/*
template<typename TCharType>
void BasicTokenizer<TCharType>::Start(File &cFile) {
  // start tokenizer
  stop();
  m_pStream = new BasicBufferedReaderFile<TCharType>(cFile);
}
*/

template<typename TCharType>
void BasicTokenizer<TCharType>::stop() {
  // stop tokenizer
  if (m_pStream) {
    delete m_pStream;
    m_pStream = nullptr;
  }
  m_sToken     = "";
  m_nPosition  = 0;
  m_nLine      = 0;
  m_nParseMode = eEatGarbage;
  m_sEndTag    = "";
  while (!m_cStateStack.empty()) {
    m_cStateStack.pop();
  }
}

template<typename TCharType>
core::Vector<BasicString<TCharType>> BasicTokenizer<TCharType>::get_tokens() {
  // Read tokens
  core::Vector<BasicString<TCharType> > cTokens;
  BasicString<TCharType> sToken = get_next_token();
  while (sToken.length()) {
    // Add token
    cTokens.push_back(sToken);

    // Next token
    sToken = get_next_token();
  }

  // Return array of tokens
  return cTokens;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_next_token() {
  // Clear the current token
  m_sToken = "";

  // Valid stream?
  if (m_pStream) {
    // Read ASCII input
    while (!m_pStream->is_eof()) {
      // get character
      const char nChar = static_cast<char>(m_pStream->get_char());

      // Do the work
      switch (m_nParseMode) {
        // Eat garbage until the first character of a word has been found
        case eEatGarbage:
          if (m_sCommentStartTag.length() && stream_is_string(m_sCommentStartTag)) {
            // The beginning of a comment has been found
            stream_read(m_sCommentStartTag.length());
            m_sEndTag	 = m_sCommentEndTag;
            m_nParseMode = eSkipComment;
          } else if (m_sSingleLineComment.length() && stream_is_string(m_sSingleLineComment)) {
            // The beginning of a comment has been found
            stream_read(m_sSingleLineComment.length());
            m_sEndTag	 = EndOfLine;
            m_nParseMode = eSkipComment;
          } else if (m_sQuotes.find(nChar) != BasicString<TCharType>::NPOS) {
            // The beginning of a quote has been found
            stream_read(1);
            m_sEndTag	 = nChar;
            m_nParseMode = eReadQuote;
          } else if (m_sDelimiters.find(nChar) != BasicString<TCharType>::NPOS) {
            // Only more garbage found
            stream_read(1);
          } else if (m_sSingleChars.find(nChar) != BasicString<TCharType>::NPOS) {
            // A single character word has been found
            m_nParseMode = eSingleChar;
          } else {
            // The beginning of a word has been found
            m_nParseMode = eReadWord;
          }
          break;

          // Read a word until a delimiter has been found
        case eReadWord:
          if (m_sCommentStartTag.length() && stream_is_string(m_sCommentStartTag)) {
            // The beginning of a comment has been found
            stream_read(m_sCommentStartTag.length());
            m_sEndTag	 = m_sCommentEndTag;
            m_nParseMode = eSkipComment;
          } else if (m_sSingleLineComment.length() && stream_is_string(m_sSingleLineComment)) {
            // The beginning of a comment has been found
            stream_read(m_sSingleLineComment.length());
            m_sEndTag	 = EndOfLine;
            m_nParseMode = eSkipComment;
          } else if (m_sQuotes.find(nChar) != BasicString<TCharType>::NPOS) {
            // The beginning of a quote has been found
            stream_read(1);
            m_sEndTag	 = nChar;
            m_nParseMode = eReadQuote;
          } else if (m_sDelimiters.find(nChar) != BasicString<TCharType>::NPOS) {
            // A delimiter has been found
            stream_read(1);
            m_nParseMode = eEatGarbage;
          } else if (m_sSingleChars.find(nChar) != BasicString<TCharType>::NPOS) {
            // A single character word has been found
            m_nParseMode = eSingleChar;
          } else {
            // Read word
            m_sToken += nChar;
            stream_read(1);
          }

          // Return word if one has been found
          if (m_nParseMode != eReadWord && m_sToken.length())
            return m_sToken;
          break;

          // Read a single character word
        case eSingleChar:
          // Return the single character word
          stream_read(1);
          m_nParseMode = eEatGarbage;
          m_sToken	 = nChar;
          return m_sToken;

          // Read the beginning of a quote
        case eReadQuote:
          m_nParseMode = eReadQuoteInside;
          m_sToken	 = m_sEndTag;
          return m_sEndTag;

          // Read until the end quote has been found
        case eReadQuoteInside:
          if (stream_is_string(m_sEndTag)) {
            // Return string inside the quote
            m_nParseMode = eReadQuoteEnd;
            if (m_sToken.length())
              return m_sToken;
          } else {
            m_sToken += nChar;
            stream_read(1);
          }
          break;

          // Read the end of a quote
        case eReadQuoteEnd:
          stream_read(m_sEndTag.length());
          m_nParseMode = eEatGarbage;
          m_sToken	 = m_sEndTag;
          return m_sEndTag;

          // Skip a comment until the comment end-tag has been found
        case eSkipComment:
          if (stream_is_string(m_sEndTag)) {
            stream_read(m_sEndTag.length());
            m_nParseMode = eEatGarbage;
          } else {
            stream_read(1);
          }
          break;
      }
    }
  }

  // Return token
  return m_sToken;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::expect_token(const BasicString<TCharType> &sExpected) {
  push_state();
  const BasicString<TCharType> sToken = get_next_token();
  if (!sToken.length() || !compare_token(sExpected)) {
    pop_state();

    // The next token is not equal to the string
    return false;
  } else {
    drop_state();

    // The next token is equal to the string
    return true;
  }
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::find_token(const BasicString<TCharType> &sExpected) {
  // Check expected string
  if (sExpected.length()) {
    // Read first token
    push_state();
    BasicString<TCharType> sToken = get_next_token();

    // Read tokens
    while (sToken.length()) {
      // Token found?
      if (compare_token(sExpected)) {
        pop_state();
        return true;
      }

      // Next token
      drop_state();
      push_state();
      sToken = get_next_token();
    }
    drop_state();
  }

  // Not found
  return false;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::get_token() const {
  // Return current token
  return m_sToken;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::compare_token(const BasicString<TCharType> &sExpected) {
  return (m_bCaseSensitive ? m_sToken.compare(sExpected) : m_sToken.comparei(sExpected));
}

template<typename TCharType>
uint32 BasicTokenizer<TCharType>::get_position() const {
  // Return current position
  return m_nPosition;
}

template<typename TCharType>
uint32 BasicTokenizer<TCharType>::get_line() const {
  // Return current line
  return m_nLine;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::push_state() {
  // Push current state onto the stack
  STokenizerState sState;
  sState.m_sToken		= m_sToken;
  sState.m_nPosition	= m_nPosition;
  sState.m_nLine		= m_nLine;
  sState.m_nParseMode	= m_nParseMode;
  sState.m_sEndTag	= m_sEndTag;
  m_cStateStack.Push(sState);
}

template<typename TCharType>
void BasicTokenizer<TCharType>::pop_state() {
  // Is the stack empty?
  if (m_cStateStack.size() > 0) {
    // Restore state
    STokenizerState sState = m_cStateStack.Pop();
    m_sToken	 = sState.m_sToken;
    m_nPosition	 = sState.m_nPosition;
    m_nLine		 = sState.m_nLine;
    m_nParseMode = sState.m_nParseMode;
    m_sEndTag	 = sState.m_sEndTag;
    if (m_pStream)
      m_pStream->seek(m_nPosition);
  }
}

template<typename TCharType>
void BasicTokenizer<TCharType>::drop_state() {
  // Is the stack empty?
  if (m_cStateStack.size() > 0) {
    // Pop saved state from stack
    m_cStateStack.Pop();
  }
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_number(int &nNumber) {
  const BasicString<TCharType> sToken = get_next_token();
  if (sToken.length()) {
    nNumber = sToken.get_int();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_number(float &fNumber) {
  const BasicString<TCharType> sToken = get_next_token();
  if (sToken.length()) {
    fNumber = sToken.get_float();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_number(double &dNumber) {
  const BasicString<TCharType> sToken = get_next_token();
  if (sToken.length()) {
    dNumber = sToken.get_double();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool
BasicTokenizer<TCharType>::parse_vector(core::Vector<BasicString<TCharType>> &cVector, const BasicString<TCharType> &sStart,
                                        const BasicString<TCharType> &sEnd, const BasicString<TCharType> &sSeparator) {
  // start parsing
  push_state();

  // Expect open bracket
  if (expect_token(sStart)) {
    // Read elements
    bool bFirst = true;
    while (!expect_token(sEnd)) {
      // Expect separator
      if (bFirst) {
        bFirst = false;
      } else if (sSeparator.length() && !expect_token(sSeparator)) {
        pop_state();

        // Error!
        return false;
      }

      // Expect an element
      BasicString<TCharType> sElement = get_next_token();
      if (!sElement.length() || compare_token(sSeparator) || compare_token(sEnd)) {
        // Error!
        pop_state();
        return false;
      }
      cVector.Add(sElement);
    }

    // Success
    drop_state();
    return true;
  } else {
    pop_state();

    // Error!
    return false;
  }
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_vector(core::Vector<int> &cVector, const BasicString<TCharType> &sStart,
                                             const BasicString<TCharType> &sEnd,
                                             const BasicString<TCharType> &sSeparator) {
  // Parse string vector
  core::Vector<BasicString<TCharType> > cStringVector;
  if (!parse_vector(cStringVector, sStart, sEnd, sSeparator))
    return false; // Error!

  // Create int vector
  for (uint32 i=0; i<cStringVector.size(); i++)
    cVector.push_back(cStringVector[i].get_int());

  // Done
  return true;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_vector(core::Vector<float> &cVector, const BasicString<TCharType> &sStart,
                                             const BasicString<TCharType> &sEnd,
                                             const BasicString<TCharType> &sSeparator) {
  // Parse string vector
  core::Vector<BasicString<TCharType> > cStringVector;
  if (!parse_vector(cStringVector, sStart, sEnd, sSeparator))
    return false; // Error!

  // Create int vector
  for (uint32 i=0; i<cStringVector.size(); i++)
    cVector.push_back(cStringVector[i].get_float());

  // Done
  return true;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseVector(core::Vector<double> &cVector, const BasicString<TCharType> &sStart,
                                            const BasicString<TCharType> &sEnd,
                                            const BasicString<TCharType> &sSeparator) {
  // Parse string vector
  core::Vector<BasicString<TCharType> > cStringVector;
  if (!parse_vector(cStringVector, sStart, sEnd, sSeparator))
    return false; // Error!

  // Create int vector
  for (uint32 i=0; i<cStringVector.size(); i++)
    cVector.push_back(cStringVector[i].get_double());

  // Done
  return true;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_equation(BasicString<TCharType> &sName, BasicString<TCharType> &sValue,
                                               const BasicString<TCharType> &sEquation) {
  // start parsing
  push_state();

  // Expect name
  BasicString<TCharType> sToken = get_next_token();
  if (sToken.length()) {
    // Store name
    sName = sToken;

    // Expect equation
    sToken = get_next_token();
    if (sToken.length()) {
      if (!sEquation.length() || compare_token(sEquation)) {
        // Read value
        sToken = get_next_token();
        if (sToken.length()) {
          // Store value
          sValue = sToken;
          drop_state();

          // Done
          return true;
        }
      }
    }
  }

  // An error has occurred
  pop_state();
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_equation(BasicString<TCharType> &sName, int &nValue,
                                               const BasicString<TCharType> &sEquation) {
  BasicString<TCharType> sValue;
  if (parse_equation(sName, sValue, sEquation)) {
    nValue = sValue.get_int();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_equation(BasicString<TCharType> &sName, float &fValue,
                                               const BasicString<TCharType> &sEquation) {
  BasicString<TCharType> sValue;
  if (parse_equation(sName, sValue, sEquation)) {
    fValue = sValue.get_float();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::parse_equation(BasicString<TCharType> &sName, double &dValue,
                                               const BasicString<TCharType> &sEquation) {
  BasicString<TCharType> sValue;
  if (parse_equation(sName, sValue, sEquation)) {
    dValue = sValue.get_double();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
BasicTokenizer<TCharType>::BasicTokenizer(const BasicTokenizer<TCharType> &cSource) :
  m_sDelimiters(" \t\r\n"),
  m_sSingleChars("{}()[]<*>/=,;\""),
  m_sQuotes("\"\'"),
  m_sCommentStartTag("/*"),
  m_sCommentEndTag("*/"),
  m_sSingleLineComment("//"),
  m_bCaseSensitive(false),
  m_pStream(nullptr),
  m_nPosition(0),
  m_nLine(0),
  m_nParseMode(eEatGarbage) {

}

template<typename TCharType>
BasicTokenizer<TCharType> &BasicTokenizer<TCharType>::operator=(const BasicTokenizer<TCharType> &cSource) {
  return *this;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::stream_is_string(const BasicString<TCharType> &sString) {
  if (!m_pStream->is_eof())
    return (m_bCaseSensitive ? m_pStream->is_string(sString) : m_pStream->is_string_no_case(sString));
  else
    return false;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::stream_read(uint32 nSize) {
  // Read next character(s)
  for (uint32 i=0; i<nSize; i++) {
    if (m_pStream->read_char() == '\n')
      m_nLine++;
  }
  m_nPosition += nSize;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]