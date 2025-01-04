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
#include "core/log/log_formatter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Text log formatter
*
*  @remarks
*    The log is written in pure text to the file.
*
*  @note
*    - Implementation of the strategy design pattern, this class a concrete strategy of the strategy "LogFormatter" of the context "Log"
*/
class LogFormatterText : public LogFormatter {


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  CORE_API LogFormatterText();

  /**
  *  @brief
  *    Destructor
  */
  CORE_API ~LogFormatterText() override;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    Source to copy from
  */
  LogFormatterText(const LogFormatterText &cSource);

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] cSource
  *    Source to copy from
  *
  *  @return
  *    Reference to this instance
  */
  LogFormatterText &operator=(const LogFormatterText &cSource);


  //[-------------------------------------------------------]
  //[ Private virtual LogFormatter functions                ]
  //[-------------------------------------------------------]
private:
  bool output(uint8 nLogLevel, const String &sText) override;

  bool flush() override;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
