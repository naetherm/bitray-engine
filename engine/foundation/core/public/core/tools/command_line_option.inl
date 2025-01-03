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
inline CommandLineOption::CommandLineOption() :
  mType(OptionParam),
  mRequired(false) {
}

inline CommandLineOption::~CommandLineOption() {
}

inline CommandLineOption::EType CommandLineOption::get_type() const {
  // Return type
  return mType;
}

inline void CommandLineOption::set_type(EType nType) {
  // Set type
  mType = nType;
}

inline bool CommandLineOption::is_required() const {
  // Return required-flag
  return mRequired;
}

inline void CommandLineOption::set_required(bool bRequired) {
  // Set required-flag
  mRequired = bRequired;
}

inline String CommandLineOption::get_name() const {
  // Return name
  return mName;
}

inline void CommandLineOption::set_name(const String &sName) {
  // Set name
  mName = sName;
}

inline String CommandLineOption::get_short_name() const {
  // Return short name
  return mShortName;
}

inline void CommandLineOption::set_short_name(const String &sName) {
  // Set short name
  mShortName = sName;
}

inline String CommandLineOption::get_long_name() const {
  // Return long name
  return mLongName;
}

inline void CommandLineOption::set_long_name(const String &sName) {
  // Set long name
  mLongName = sName;
}

inline String CommandLineOption::get_description() const {
  // Return description
  return mDescription;
}

inline void CommandLineOption::set_description(const String &sDescription) {
  // Set description
  mDescription = sDescription;
}

inline String CommandLineOption::get_default() const {
  // Return default value
  return mDefault;
}

inline void CommandLineOption::set_default(const String &sDefault) {
  // Set default value
  mDefault = sDefault;
}

inline bool CommandLineOption::is_set() const {
  return (mValue.length() > 0);
}

inline String CommandLineOption::get_value() const {
  // Return value
  return mValue;
}

inline void CommandLineOption::set_value(const String &sValue) {
  // Set value
  mValue = sValue;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
