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
template<typename TType, typename TContainer>
Stack<TType, TContainer>::Stack()
: mData() {

}

template<typename TType, typename TContainer>
Stack<TType, TContainer>::Stack(const container_type &rhs)
: mData(rhs) {

}

template<typename TType, typename TContainer>
Stack<TType, TContainer>::Stack(container_type &&rhs)
: mData(core::move(rhs)){

}

template<typename TType, typename TContainer>
bool Stack<TType, TContainer>::empty() const {
  return mData.empty();
}

template<typename TType, typename TContainer>
typename Stack<TType, TContainer>::size_type Stack<TType, TContainer>::size() const {
  return mData.size();
}

template<typename TType, typename TContainer>
typename Stack<TType, TContainer>::reference Stack<TType, TContainer>::top() {
  return mData.back();
}

template<typename TType, typename TContainer>
typename Stack<TType, TContainer>::const_reference Stack<TType, TContainer>::top() const {
  return mData.back();
}

template<typename TType, typename TContainer>
void Stack<TType, TContainer>::push(const value_type &element) {
  mData.push_back(const_cast<value_type&>(element));
}

template<typename TType, typename TContainer>
void Stack<TType, TContainer>::push(value_type &&element) {
  mData.push_back(core::move(element));
}

template<typename TType, typename TContainer>
template<typename... TArgs>
void Stack<TType, TContainer>::emplace_back(TArgs &&... args) {
  emplace(core::forward<TArgs>(args)...);
}

template<typename TType, typename TContainer>
template<typename... TArgs>
decltype(auto) Stack<TType, TContainer>::emplace(TArgs &&... args) {
  return mData.emplace_back(core::forward<TArgs>(args)...);
}

template<typename TType, typename TContainer>
void Stack<TType, TContainer>::pop() {
  mData.pop_back();
}

template<typename TType, typename TContainer>
typename Stack<TType, TContainer>::container_type &Stack<TType, TContainer>::get_container() {
  return mData;
}

template<typename TType, typename TContainer>
const typename Stack<TType, TContainer>::container_type &Stack<TType, TContainer>::get_container() const {
  return mData;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core