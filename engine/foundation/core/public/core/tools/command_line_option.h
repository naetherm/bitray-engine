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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @brief
 * Command line option
 */
class CommandLineOption {


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Command line option type
   */
  enum EType {
    /** A flag, e.g. 'command -v' or 'command --verbose' */
    OptionFlag,
    /** A parameter, e.g. 'command -f name' or 'command --filename name' */
    OptionParam,
    /** An argument, e.g. 'command name' */
    OptionArgument
  };


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   */
  inline CommandLineOption();

  /**
   * @brief
   * Destructor
   */
  inline ~CommandLineOption();

  /**
   * @brief
   * get option type
   *
   * @return
   * Type of option
   */
  [[nodiscard]] inline EType get_type() const;

  /**
   * @brief
   * Set option type
   *
   * @param[in] nType
   * Type of option
   */
  inline void set_type(EType nType);

  /**
   * @brief
   * Check if this option is required
   *
   * @return
   * 'true' if the option is required, else 'false'
   */
  [[nodiscard]] inline bool is_required() const;

  /**
   * @brief
   * Set if this option is required
   *
   * @param[in] bRequired
   * 'true' if the option is required, else 'false'
   */
  inline void set_required(bool bRequired);

  /**
   * @brief
   * get name
   *
   * @return
   * Option name (e.g. "Name")
   */
  [[nodiscard]] inline String get_name() const;

  /**
   * @brief
   * Set name
   *
   * @param[in] sName
   * Option name (e.g. "Name")
   */
  inline void set_name(const String &sName);

  /**
   * @brief
   * get short name
   *
   * @return
   * Short option name (e.g. "-a")
   */
  [[nodiscard]] inline String get_short_name() const;

  /**
   * @brief
   * Set short name
   *
   * @param[in] sName
   * Short option name (e.g. "-a")
   */
  inline void set_short_name(const String &sName);

  /**
   * @brief
   * get long name
   *
   * @return
   * Long option name (e.g. "--optiona")
   */
  [[nodiscard]] inline String get_long_name() const;

  /**
   * @brief
   * Set long name
   *
   * @param[in] sName
   * Long option name (e.g. "--optiona")
   */
  inline void set_long_name(const String &sName);

  /**
   * @brief
   * get description
   *
   * @return
   * Description string
   */
  [[nodiscard]] inline String get_description() const;

  /**
   * @brief
   * Set description
   *
   * @param[in] sDescription
   * Description string
   */
  inline void set_description(const String &sDescription);

  /**
   * @brief
   * get default value
   *
   * @return
   * Default value
   */
  [[nodiscard]] inline String get_default() const;

  /**
   * @brief
   * Set default value
   *
   * @param[in] sDefault
   * Default value
   */
  inline void set_default(const String &sDefault);

  /**
   * @brief
   * Check if option value is set (either by command line or by default value)
   *
   * @return
   * 'true' if the value is set, else 'false'
   */
  [[nodiscard]] inline bool is_set() const;

  /**
   * @brief
   * get option value
   *
   * @return
   * Value of option ("true"/"false" for boolean values)
   */
  [[nodiscard]] inline String get_value() const;

  /**
   * @brief
   * Set value
   *
   * @param[in] sValue
   * Option value
   */
  inline void set_value(const String &sValue);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /** Type of command line option */
  EType mType;
  /** Is this option required? */
  bool mRequired;
  /** Name, can be "" except for OptionArgument */
  String mName;
  /** Short name (e.g. "-a"), can be "" */
  String mShortName;
  /** Long name (e.g. "--optiona"), can be "" */
  String mLongName;
  /** Description */
  String mDescription;
  /** Default value */
  String mDefault;
  /** Current value */
  String mValue;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/tools/command_line_option.inl"
