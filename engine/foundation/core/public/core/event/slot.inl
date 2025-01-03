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
Slot<TArgs...>::Slot() {

}

template<typename... TArgs>
template<typename TClass>
Slot<TArgs...>::Slot(TClass* cls, void(TClass::*METHOD)(TArgs...)) {
  mCallback = [cls, METHOD](TArgs... args) {
    (cls->*METHOD)(args...);
  };
}

template<typename... TArgs>
template<typename TClass>
Slot<TArgs...>::Slot(TClass* cls, void(TClass::*METHOD)(TArgs...) const) {
  mCallback = [cls, METHOD](TArgs... args) {
    (cls->*METHOD)(args...);
  };
}

template<typename... TArgs>
Slot<TArgs...>::Slot(const Slot& rhs)
: mCallback(rhs.mCallback) {

}

template<typename... TArgs>
Slot<TArgs...>::~Slot() {

}

template<typename... TArgs>
Slot<TArgs...>& Slot<TArgs...>::operator=(const Slot& rhs) {
  mCallback = rhs.mCallback;
  return *this;
}

template<typename... TArgs>
bool Slot<TArgs...>::operator==(const Slot& rhs) const {
  return mCallback == rhs.mCallback;
}

template<typename... TArgs>
void Slot<TArgs...>::call(TArgs... args) {
  invoke(nullptr, args...);
}

template<typename... TArgs>
void Slot<TArgs...>::invoke(void* obj, TArgs... args) const {
  mCallback(args...);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core