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
#include "core/utility/invokable.h"
#include "core/container/vector.h"
#include "core/event/slot.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Signal
 *
 * @tparam TArgs
 *
 * @brief
 * Signal implementation, which is connected to Slots.
 */
template<typename... TArgs>
class Signal : public Invokable<void, TArgs...> {
public:

  typedef Slot<TArgs...> SlotType;

public:
  /**
   * @brief
   * Constructor.
   */
  Signal();

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  Signal(const Signal& rhs);

  /**
   * @brief
   * Destructor.
   */
  ~Signal() override;


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Object to copy.
   *
   * @return
   * Reference to this instance.
   */
  Signal& operator=(const Signal& rhs);

public:
  /**
   * @brief
   * Invokes the signal, by that calls all slots.
   *
   * @tparam[in] args
   * The arguments to send to the slots.
   */
  void invoke(TArgs... args) const override;

  /**
   * @brief
   * Connects the given slot to this signal.
   *
   * @param[in] slot
   * Pointer to the slot to connect this signal.
   */
  void connect(SlotType* slot);

  /**
   * @brief
   * Disconnects the given slot to this signal.
   *
   * @param[in] slot
   * Pointer to the slot to disconnect this signal.
   */
  void disconnect(SlotType* slot);

  /**
   * @brief
   * Disconnects all slots.
   */
  void disconnect_all();

private:
  /** List of all connected slots */
  Vector<SlotType*> mAllSlots;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/event/signal.inl"