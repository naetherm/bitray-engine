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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
inline CommandLine::CommandLine() :
  mbError(false) {
}

inline CommandLine::~CommandLine() {
  // Clear options
  clear();
}

inline uint32 CommandLine::get_num_of_options() const {
  // Return number of options stored in our array
  return mlstOptions.size();
}

inline CommandLineOption *CommandLine::get_option(uint32 nIndex) const {
  // Return option
  return mlstOptions[nIndex];
}

inline CommandLineOption *CommandLine::get_option(const String &sName) const {
  // Return option
  auto iter = this->mmapOptions.find(sName);
  if (iter != this->mmapOptions.end()) {
    return iter.value();
  }
  return nullptr;
}

inline bool CommandLine::has_errors() const {
  // Return error flag
  return mbError;
}

inline uint32 CommandLine::get_num_of_additional_arguments() const {
  // Return number of additional arguments
  return mlstParameters.size();
}

inline String CommandLine::get_additional_argument(uint32 nIndex) const {
  auto iter = this->mlstParameters.begin();
  std::advance(iter, nIndex);
  return *iter;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
