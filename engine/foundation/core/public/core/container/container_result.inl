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
template<typename TIterator>
ContainerResult<TIterator>::ContainerResult()
: mSucceeded(false) {

}

template<typename TIterator>
ContainerResult<TIterator> ContainerResult<TIterator>::success(const ContainerResult<TIterator>::iterator_type &iter) {
  return ContainerResult<TIterator>(iter, true);
}

template<typename TIterator>
ContainerResult<TIterator> ContainerResult<TIterator>::failure(const ContainerResult<TIterator>::iterator_type &iter) {
  return ContainerResult<TIterator>(iter, false);
}

template<typename TIterator>
ContainerResult<TIterator> ContainerResult<TIterator>::failure() {
  return ContainerResult<TIterator>(TIterator(), false);
}

template<typename TIterator>
bool ContainerResult<TIterator>::successful() const {
  return mSucceeded;
}

template<typename TIterator>
typename ContainerResult<TIterator>::iterator_type ContainerResult<TIterator>::iterator() const {
  return mIterator;
}

template<typename TIterator>
ContainerResult<TIterator>::ContainerResult(const iterator_type &iter, bool succeeded)
: mIterator(iter)
, mSucceeded(succeeded) {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core