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
#include "core/string/tokenizer.h"
#include "core/string/regex.h"
#include "core/platform/console.h"
#include "core/tools/command_line_option.h"
#include "core/tools/command_line.h"
#include <list>


namespace {

namespace detail {

static constexpr core::uint32 LINE_LENGTH = 79;

}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Global helper functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add spaces to a string to ensure that it has a certain length
*
*  @param[in] sString
*    Input string
*  @param[in] nLength
*    Desired length of output string
*
*  @return
*    String that has been extended to the given length
*/
String SetStringLength(const String &sString, uint32 nLength) {
  String sAdd;
  for (uint32 i = sString.length(); i < nLength; i++)
    sAdd = sAdd + ' ';
  return sString + sAdd;
}

/**
*  @brief
*    get maximum of two values
*
*  @param[in] nA
*    First value
*  @param[in] nB
*    Second value
*
*  @return
*    The greater of the two values
*/
uint32 Max(uint32 nA, uint32 nB) {
  return (nA > nB ? nA : nB);
}

/**
*  @brief
*    get minimum of two values
*
*  @param[in] nA
*    First value
*  @param[in] nB
*    Second value
*
*  @return
*    The lower of the two values
*/
uint32 Min(uint32 nA, uint32 nB) {
  return (nA < nB ? nA : nB);
}

/**
*  @brief
*    Print text with automatic line breaks
*
*  @param[in] nIndent
*    Column at which the text output starts
*  @param[in] sText
*    Text to print
*/
void PrintDescription(uint32 nIndent, const String &sText) {
  // Print description
  if (nIndent + sText.length() < detail::LINE_LENGTH) {
    // Description still fits on the line, just print it out
    Console::instance().print(sText + '\n');
  } else {
    // Description is too long to fit on the line
    uint32 nPos = nIndent;
    const String sEmpty = SetStringLength("", nPos);

    // get text word for word
    core::uint32 nStartPos = 0;
    core::uint32 nIndex = 0;
    while (nIndex != core::String::NPOS) {
      nIndex = sText.find_first_of(" ", nStartPos);
      String word = sText.substr(nStartPos, nIndex);

      if (nStartPos > nIndent) {
        Console::instance().print(' ');
        nStartPos++;
      }

      // Break line when necessary
      nStartPos += word.length();
      if (nStartPos >= 100) {
        Console::instance().print('\n');
        Console::instance().print(sEmpty);
        nStartPos = nIndent + word.length();
      }

      // Print word
      Console::instance().print(word);

      //nStartPos = nIndex;
    }

    /*
		static CRegEx cRegEx("\\s*([^\\s]+)");
		uint32 nParsePos = 0;
		while (cRegEx.match(sText, nParsePos)) {
			// get next word
						 nParsePos = cRegEx.get_position();
			const String sWord     = cRegEx.get_result(0);
			if (nPos > nIndent) {
				Platform::instance().getConsole().print(' ');
				nPos++;
			}

			// Break line when necessary
			nPos += sWord.length();
			if (nPos >= 79) {
				Platform::instance().getConsole().print('\n');
				Console::instance().print(sEmpty);
				nPos = nIndent + sWord.length();
			}

			// Print word
			Console::instance().print(sWord);
		}
    */
    // Break line for next option to be consistent to the other if-branch
    Console::instance().print('\n');
  }
}


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
String CommandLine::arguments_to_string(const core::Vector<String> &lstArguments) {
  // Are there any elements within the given array?
  if (lstArguments.size()) {
    String sString;

    // Loop through all elements of the given array
    for (uint32 i = 0; i < lstArguments.size(); i++) {
      if (i > 0)
        sString += ' ';
      if (sString.find(' ') != String::NPOS) {
        sString += '\"';
        sString += lstArguments[i];
        sString += '\"';
      } else {
        sString += lstArguments[i];
      }
    }

    // Return the constructed string
    return sString;
  } else {
    // Return the constructed string - which is astoundingly empty :D
    return "";
  }
}

std::vector<String> CommandLine::string_to_arguments(const String &sCmdLine) {
  // Initialize the tokenizer
  CTokenizer cTokenizer;
  cTokenizer.start(sCmdLine);

  // Loop through all tokens
  std::vector<String> lstArray;
  String sToken = cTokenizer.get_next_token();
  while (sToken.length()) {
    // Separator found?
    if (sToken == '\'' || sToken == '\"') {
      sToken = cTokenizer.get_next_token();
      lstArray.push_back(sToken);
      sToken = cTokenizer.get_next_token();
    } else {
      lstArray.push_back(sToken);
    }

    // Next token, please
    sToken = cTokenizer.get_next_token();
  }
  cTokenizer.stop();

  // Return the constructed string array
  return lstArray;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
void CommandLine::clear() {
  // Delete options
  for (uint32 i = 0; i < mlstOptions.size(); i++)
    delete mlstOptions[i];

  // Clear lists
  mlstOptions.clear();
  mmapOptions.clear();
  mlstParameters.clear();
  mbError = false;
}

bool
CommandLine::add_parameter(const String &sName, const String &sShort, const String &sLong, const String &sDescription,
                           const String &sDefault, bool bRequired) {
  // Check that a name and at least one flag name is given and the names have not been used before
  if ((sName.length() && mmapOptions.find(sName) == this->mmapOptions.end()) && (sShort.length() || sLong.length()) &&
      (sShort.length() == 0 || mmapOptions.find(sShort) == this->mmapOptions.end()) &&
      (sLong.length() == 0 || mmapOptions.find(sLong) == this->mmapOptions.end())) {
    // Create option
    CommandLineOption *pOption = new CommandLineOption();
    pOption->set_type(CommandLineOption::OptionParam);
    pOption->set_required(bRequired);
    pOption->set_name(sName);
    pOption->set_short_name(sShort);
    pOption->set_long_name(sLong);
    pOption->set_description(sDescription);
    pOption->set_default(sDefault);

    // Add option to lists
    mlstOptions.push_back(pOption);
    mmapOptions.insert(sName, pOption);
    if (sShort.length())
      mmapOptions.insert(sShort, pOption);
    if (sLong.length())
      mmapOptions.insert(sLong, pOption);
    return true;
  }

  // Error, wrong arguments given
  return false;
}

bool CommandLine::add_flag(const String &sName, const String &sShort, const String &sLong, const String &sDescription,
                           bool bRequired) {
  // Check that a name and at least one flag name is given and the names have not been used before
  if ((sName.length() && mmapOptions.find(sName) == this->mmapOptions.end()) && (sShort.length() || sLong.length()) &&
      (sShort.length() == 0 || mmapOptions.find(sShort) == this->mmapOptions.end()) &&
      (sLong.length() == 0 || mmapOptions.find(sLong) == this->mmapOptions.end())) {
    // Create option
    CommandLineOption *pOption = new CommandLineOption();
    pOption->set_type(CommandLineOption::OptionFlag);
    pOption->set_required(bRequired);
    pOption->set_name(sName);
    pOption->set_short_name(sShort);
    pOption->set_long_name(sLong);
    pOption->set_description(sDescription);

    // Add option to lists
    mlstOptions.push_back(pOption);
    mmapOptions.insert(sName, pOption);
    if (sShort.length())
      mmapOptions.insert(sShort, pOption);
    if (sLong.length())
      mmapOptions.insert(sLong, pOption);
    return true;
  }

  // Error, wrong arguments given
  return false;
}

bool
CommandLine::add_argument(const String &sName, const String &sDescription, const String &sDefault, bool bRequired) {
  // Check that a name is given and has not been used before
  if (sName.length() && mmapOptions.find(sName) == this->mmapOptions.end()) {
    // Create option
    CommandLineOption *pOption = new CommandLineOption();
    pOption->set_type(CommandLineOption::OptionArgument);
    pOption->set_required(bRequired);
    pOption->set_name(sName);
    pOption->set_description(sDescription);
    pOption->set_default(sDefault);

    // Add option to lists
    mlstOptions.push_back(pOption);
    if (sName.length())
      mmapOptions.insert(sName, pOption);
    return true;
  }

  // Error, wrong arguments given
  return false;
}

bool CommandLine::parse_command_line(const core::Vector<String> &lstArgs) {
  // Clear parameters
  mlstParameters.clear();

  // Set default values and get enumerate arguments
  std::list<CommandLineOption *> lstArguments;
  for (uint32 i = 0; i < mlstOptions.size(); i++) {
    // Set default value
    mlstOptions[i]->set_value(mlstOptions[i]->get_default());

    // Add arguments to list
    if (mlstOptions[i]->get_type() == CommandLineOption::OptionArgument)
      lstArguments.push_back(mlstOptions[i]);
  }

  // Loop through arguments
  mbError = false;
  uint32 nArgument = 0;
  CommandLineOption *pLastOption = nullptr;
  String sLastOption;
  for (uint32 i = 0; i < lstArgs.size(); i++) {
    // get argument
    String sArg = lstArgs[i];
    String sOption = "";
    String sOptions = "";
    String sValue = "";
    if (sArg.substr(0, 2) == "--") {
      // Long option found
      sOptions = "";
      sOption = sArg;
      sValue = "";
    } else if (sArg.substr(0, 1) == '-') {
      // Short option(s) found
      sOptions = sArg.substr(2);
      sOption = sArg.substr(0, 2);
      sValue = "";
    } else {
      sOptions = "";
      sOption = "";
      sValue = sArg;
    }

    // get options
    while (sOption.length()) {
      // Process option
      if (pLastOption) {
        // Error: There was still an option waiting for it's value!
        Console::instance().print("Parameter " + sLastOption + " needs a value!\n");
        mbError = true;
        return mbError;
      } else {
        // get option
        pLastOption = get_option(sOption);
        sLastOption = sOption;
        if (!pLastOption) {
          // Error: Unknown option!
          Console::instance().print("Unknown parameter " + sOption + "!\n");
          mbError = true;
          return mbError;
        }

        // Set 'true' for boolean options or wait for a value
        if (pLastOption->get_type() == CommandLineOption::OptionFlag) {
          pLastOption->set_value("true");
          pLastOption = nullptr;
          sLastOption = "";
        }
      }

      // Next option available?
      if (sOptions.length()) {
        // Yes, get next option
        sOption = '-' + sOptions.substr(0, 1);
        sOptions = sOptions.substr(1);
      } else {
        // No more options
        sOption = "";
      }
    }

    // get value
    if (sValue.length()) {
      // Process value
      if (pLastOption) {
        // Set option value
        pLastOption->set_value(sValue);
        pLastOption = nullptr;
        sLastOption = "";
      } else {
        // Argument
        if (nArgument < lstArguments.size()) {
          // Set value of defined argument
          auto iter = lstArguments.begin();
          std::advance(iter, nArgument);
          (*iter)->set_value(sValue);
          nArgument++;
        } else {
          // Add additional argument
          mlstParameters.push_back(sValue);
        }
      }
    }
  }

  // Produce an error if not all required options have been set
  for (uint32 i = 0; i < mlstOptions.size() && !mbError; i++) {
    // Check if option has been set
    if (mlstOptions[i]->is_required() && !mlstOptions[i]->is_set())
      mbError = true;
  }

  // Return error-status
  return mbError;
}

bool CommandLine::is_value_set(const String &sName) const {
  // get option
  const CommandLineOption *pOption = get_option(sName);
  return pOption ? pOption->is_set() : false;
}

String CommandLine::get_value(const String &sName) const {
  // get option
  const CommandLineOption *pOption = get_option(sName);
  return pOption ? pOption->get_value() : "";
}

void CommandLine::print_help(const String &sProgramName) const {
  // Print synopsis
  Console::instance().print("Usage: " + sProgramName + " [OPTIONS]");
  for (uint32 i = 0; i < mlstOptions.size(); i++) {
    const CommandLineOption *pOption = mlstOptions[i];
    if (pOption->get_type() == CommandLineOption::OptionArgument) {
      if (pOption->is_required())
        Console::instance().print(" <" + pOption->get_name() + ">");
      else
        Console::instance().print(" [" + pOption->get_name() + "]");
    }
  }
  Console::instance().print("\n\n");

  // Do not print anything, if no options are available
  if (mlstOptions.size() > 0) {
    // get maximum size of option names
    uint32 nMaxName = 1;
    uint32 nMaxShort = 2;
    uint32 nMaxLong = 4;
    uint32 nMaxAll = 1;
    for (uint32 i = 0; i < mlstOptions.size(); i++) {
      const CommandLineOption *pOption = mlstOptions[i];
      if (pOption->get_type() == CommandLineOption::OptionFlag) {
        nMaxShort = Max(pOption->get_short_name().length(), nMaxShort);
        nMaxLong = Max(pOption->get_long_name().length(), nMaxLong);
      } else if (pOption->get_type() == CommandLineOption::OptionParam) {
        nMaxShort = Max(pOption->get_short_name().length(), nMaxShort);
        nMaxLong = Max(pOption->get_long_name().length(), nMaxLong);
        nMaxName = Max(pOption->get_name().length(), nMaxName);
      } else if (pOption->get_type() == CommandLineOption::OptionArgument) {
        nMaxAll = Max(pOption->get_name().length(), nMaxAll);
      }
    }
    nMaxAll = Max(nMaxShort + nMaxLong + nMaxName + 5, nMaxAll);

    // Print arguments
    Console::instance().print("Arguments:\n");
    for (uint32 i = 0; i < mlstOptions.size(); i++) {
      // get option
      CommandLineOption *pOption = mlstOptions[i];
      if (pOption->get_type() == CommandLineOption::OptionArgument) {
        // Print option
        String sInfo = "  " + SetStringLength(pOption->get_name(), nMaxAll) + "  ";
        Console::instance().print(sInfo);

        // Print description
        PrintDescription(sInfo.length(), pOption->get_description());
      }
    }
    Console::instance().print('\n');

    // Print options
    Console::instance().print("Available options:\n");
    for (uint32 i = 0; i < mlstOptions.size(); i++) {
      // get option
      CommandLineOption *pOption = mlstOptions[i];
      if (pOption->get_type() == CommandLineOption::OptionFlag ||
          pOption->get_type() == CommandLineOption::OptionParam) {
        // Print option
        String sName = pOption->get_name();
        bool bComma = (pOption->get_short_name().length() > 0 && pOption->get_long_name().length() > 0);
        String sShort = SetStringLength(pOption->get_short_name(), nMaxShort);
        String sLong = SetStringLength(pOption->get_long_name(), nMaxLong);
        String sInfo = sShort + (bComma ? ", " : "  ") + sLong;
        if (pOption->get_type() == CommandLineOption::OptionParam)
          sInfo += " <" + sName + ">";
        sInfo = "  " + SetStringLength(sInfo, nMaxAll) + "  ";
        Console::instance().print(sInfo);

        // Print description
        PrintDescription(sInfo.length(), pOption->get_description());
      }
    }
    Console::instance().print('\n');
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
