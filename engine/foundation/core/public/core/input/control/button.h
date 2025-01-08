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
#include "core/input/control/control.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Button
 *
 * @brief
 * Button control.
 */
class Button : public Control {
public:
  /**
   * @brief
   * Constructor.
   *
   * @param[in] controller
   * The owning controller.
   * @param[in] name
   * The name.
   * @param[in] description
   * The description.
   * @param[in] character
   * The character.
   */
  Button(Controller& controller, const String& name, const String& description, char character = '\0');
  
  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] button
   * Object to copy.
   */
  explicit Button(const Button& button);
  
  /**
   * @brief
   * Destructor.
   */
  ~Button() override;
  
  
  /**
   * @brief
   * Copy operator.
   *
   * @param[in] button
   * Object to copy.
   *
   * @return
   * Reference to this instance.
   */
  Button& operator=(const Button& button);
  
  [[nodiscard]] bool operator==(const Button& button) const;

  [[nodiscard]] bool operator!=(const Button& button) const;
  
  
  /**
   * @brief
   * Returns the character, represented by this button.
   *
   * @return
   * The character.
   */
  [[nodiscard]] inline char get_character() const;
  
  /**
   * @brief
   * Returns whether the button was pressed.
   *
   * @return
   * 'true' if the button was pressed.
   */
  [[nodiscard]] inline bool is_pressed() const;
  
  /**
   * @brief
   * Sets whether this button is currently pressed.
   *
   * @param[in] pressed
   * Is this button pressed.
   */
  inline void set_pressed(bool pressed);
  
  /**
   * @brief
   * Returns whether the button was hit.
   *
   * @return
   * 'true' if button was hit.
   */
  [[nodiscard]] inline bool is_hit() const;
  
  /**
   * @brief
   * Returns whether button was hit and resets its state.
   *
   * @return
   * 'true' if button was hit.
   */
  [[nodiscard]] inline bool check_hit_and_release();
  
private:
  /** The character associated with the button */
  char mCharacter;
  /** True if the button was pressed */
  bool mPressed;
  /** True if the button was hit */
  bool mHit;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/input/control/button.inl"