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
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename... TArgs>
Signal<TArgs...>::Signal() {

}

template<typename... TArgs>
Signal<TArgs...>::Signal(const Signal& rhs)
: mAllSlots(rhs.mAllSlots) {

}

template<typename... TArgs>
Signal<TArgs...>::~Signal() {
  disconnect_all();
}

template<typename... TArgs>
Signal<TArgs...>& Signal<TArgs...>::operator=(const Signal& rhs) {
  mAllSlots = rhs.mAllSlots;
}


template<typename... TArgs>
void Signal<TArgs...>::invoke(TArgs... args) const {
  for (auto slot: mAllSlots) {
    slot->call(args...);
  }
}

template<typename... TArgs>
void Signal<TArgs...>::connect(SlotType *slot) {
  mAllSlots.push_back(slot);
}

template<typename... TArgs>
void Signal<TArgs...>::disconnect(SlotType *slot) {
  mAllSlots.erase(slot);
}

template<typename... TArgs>
void Signal<TArgs...>::disconnect_all() {
  mAllSlots.clear();
}

template<typename... TArgs>
FunctionSignature Signal<TArgs...>::get_signature() const {
  return FunctionSignature::from_template<void, TArgs...>();
}



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core