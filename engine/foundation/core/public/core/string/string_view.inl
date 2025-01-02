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
#include "core/std/algorithm.h"
#include "core/std/limits.h"
#include "core/std/algorithm.h"
#include "core/string/string_utils.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView()
  : mpBegin(nullptr), mnCount(0) {

}

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView(const BasicStringView &cSource)
  : mpBegin(cSource.mpBegin), mnCount(cSource.mnCount) {

}

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView(const TCharType *pStr, BasicStringView::size_type nCount)
  : mpBegin(pStr), mnCount(nCount) {

}

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView(const TCharType *pStr)
  : mpBegin(pStr), mnCount(pStr != nullptr ? char_strlen(pStr) : 0) {

}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference
BasicStringView<TCharType>::operator[](BasicStringView::size_type nIndex) const {
  return this->mpBegin[nIndex];
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::begin() const {
  return this->mpBegin;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::cbegin() const {
  return this->mpBegin;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::end() const {
  return this->mpBegin + this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::cend() const {
  return this->mpBegin + this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_pointer BasicStringView<TCharType>::data() const noexcept {
  return this->mpBegin;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference BasicStringView<TCharType>::front() const {
  return this->mpBegin[0];
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference BasicStringView<TCharType>::back() const {
  return this->mpBegin[this->mnCount - 1];
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference
BasicStringView<TCharType>::at(BasicStringView::size_type nIndex) const {
  return this->mpBegin[nIndex];
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type BasicStringView<TCharType>::size() const {
  return this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type BasicStringView<TCharType>::length() const {
  return this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type BasicStringView<TCharType>::max_size() const {
  return std::numeric_limits<size_type>::max();
}

template<typename TCharType>
bool BasicStringView<TCharType>::empty() const {
  return this->mnCount == 0;
}

template<typename TCharType>
void BasicStringView<TCharType>::swap(BasicStringView &cSource) {
  core::swap(this->mpBegin, cSource.mpBegin);
  core::swap(this->mnCount, cSource.mnCount);
}

template<typename TCharType>
void BasicStringView<TCharType>::remove_prefix(BasicStringView::size_type nNumElements) {
  this->mpBegin += nNumElements;
  this->mnCount -= nNumElements;
}

template<typename TCharType>
void BasicStringView<TCharType>::remove_suffix(BasicStringView::size_type nNumElements) {
  this->mnCount -= nNumElements;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::copy(TCharType *pDst, BasicStringView::size_type nCount,
                                 BasicStringView::size_type nPosition) const {
  nCount = core::MinAlt(nCount, this->mnCount - nPosition);
  auto *pResult = CharStringUninitializedCopy(this->mpBegin + nPosition, this->mpBegin + nPosition + nCount, pDst);
  return pResult - pDst;
}

template<typename TCharType>
BasicStringView<TCharType>
BasicStringView<TCharType>::substr(BasicStringView::size_type nPosition, BasicStringView::size_type nCount) {
  nCount = core::MinAlt(nCount, this->mnCount - nPosition);
  return this_type(this->mpBegin + nPosition, nCount);
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(const TCharType *pBegin1, const TCharType *pEnd1, const TCharType *pBegin2,
                                        const TCharType *pEnd2) {
  const ptrdiff_t n1 = pEnd1 - pBegin1;
  const ptrdiff_t n2 = pEnd2 - pBegin2;
  const ptrdiff_t nMin = core::MinAlt(n1, n2);
  const int cmp = core::char_string_compare(pBegin1, pBegin2, static_cast<size_type>(nMin));

  return (cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0)));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView cSource) const {
  return compare(mpBegin, mpBegin + mnCount, cSource.mpBegin, cSource.mpBegin + cSource.mnCount);
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        BasicStringView cSource) const {
  return substr(nPos1, nCount1).compare(cSource);
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        BasicStringView cSource, BasicStringView::size_type nPos2,
                                        BasicStringView::size_type nCount2) const {
  return substr(nPos1, nCount1).compare(cSource.substr(nPos2, nCount2));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(const TCharType *pSrc) {
  return compare(BasicStringView(pSrc));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        const TCharType *pStr) {
  return substr(nPos1, nCount1).compare(BasicStringView(pStr));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        const TCharType *pStr, BasicStringView::size_type nCount2) const {
  return substr(nPos1, nCount1).compare(BasicStringView(pStr, nCount2));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  auto *pEnd = mpBegin + mnCount;
  if (((NPOS - cSource.size()) >= nPosition) && (nPosition + cSource.size()) <= mnCount) {
    const value_type *const pTemp = core::search(mpBegin + nPosition, pEnd, cSource.data(),
                                                   cSource.data() + cSource.size());

    if ((pTemp != pEnd) || (cSource.size() == 0))
      return (size_type)(pTemp - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find(TCharType cChar, BasicStringView::size_type nPosition) const {
  return find(BasicStringView(&cChar, 1), nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find(const TCharType *pStr, BasicStringView::size_type nPosition,
                                 BasicStringView::size_type nCount) const {
  return find(BasicStringView(pStr, nCount), nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return find(BasicStringView(pStr), nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rfind(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return rfind(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rfind(TCharType cChar, BasicStringView::size_type nPosition) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + core::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = CharTypeStringrFind(pEnd, mpBegin, cChar);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rfind(const TCharType *pStr, BasicStringView::size_type nPosition,
                                  BasicStringView::size_type nNum) const {
  if (nNum <= mnCount) {
    if (nNum) {
      const const_iterator pEnd = mpBegin + core::MinAlt(mnCount - nNum, nPosition) + nNum;
      const const_iterator pResult = CharTypeStringRSearch(mpBegin, pEnd, pStr, pStr + nNum);

      if (pResult != pEnd)
        return (size_type)(pResult - mpBegin);
    } else
      return core::MinAlt(mnCount, nPosition);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rfind(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return rFind(pStr, nPosition, (size_type) CharStrlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_of(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return find_first_of(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_of(TCharType cChar, BasicStringView::size_type nPosition) const {
  return find(cChar, nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_of(const TCharType *pStr, BasicStringView::size_type nPosition,
                                        BasicStringView::size_type nNum) const {
  if ((nPosition < mnCount)) {
    const value_type *const pBegin = mpBegin + nPosition;
    const value_type *const pEnd = mpBegin + mnCount;
    const const_iterator pResult = char_type_string_find_first_of(pBegin, pEnd, pStr, pStr + nNum);

    if (pResult != pEnd)
      return (size_type)(pResult - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_of(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return find_first_of(pStr, nPosition, (size_type) char_strlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_of(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return find_last_of(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_of(TCharType cChar, BasicStringView::size_type nPosition) const {
  return rfind(cChar, nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_of(const TCharType *pStr, BasicStringView::size_type nPosition,
                                       BasicStringView::size_type nNum) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + core::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = char_type_string_rfind_first_of(pEnd, mpBegin, pStr, pStr + nNum);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_of(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return find_last_of(pStr, nPosition, (size_type) char_strlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_not_of(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return find_first_not_of(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_not_of(TCharType cChar, BasicStringView::size_type nPosition) const {
  if (nPosition <= mnCount) {
    const auto pEnd = mpBegin + mnCount;
    const const_iterator pResult = char_type_string_find_first_not_of(mpBegin + nPosition, pEnd, &cChar, &cChar + 1);

    if (pResult != pEnd)
      return (size_type)(pResult - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_not_of(const TCharType *pStr, BasicStringView::size_type nPosition,
                                           BasicStringView::size_type nNum) const {
  if (nPosition <= mnCount) {
    const auto pEnd = mpBegin + mnCount;
    const const_iterator pResult = char_type_string_find_first_not_of(mpBegin + nPosition, pEnd, pStr, pStr + nNum);

    if (pResult != pEnd)
      return (size_type)(pResult - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_first_not_of(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return find_first_not_of(pStr, nPosition, (size_type) char_strlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_not_of(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return find_last_not_of(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_not_of(TCharType cChar, BasicStringView::size_type nPosition) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + core::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = char_type_string_rfind_first_not_of(pEnd, mpBegin, &cChar, &cChar + 1);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_not_of(const TCharType *pStr, BasicStringView::size_type nPosition,
                                          BasicStringView::size_type nNum) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + core::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = char_type_string_rfind_first_not_of(pEnd, mpBegin, pStr, pStr + nNum);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::find_last_not_of(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return findLastNotOf(pStr, nPosition, (size_type) CharStrlen(pStr));
}

template<typename TCharType>
bool BasicStringView<TCharType>::starts_with(BasicStringView cSource) const {
  return (size() >= cSource.size()) && (compare(0, cSource.size(), cSource) == 0);
}

template<typename TCharType>
bool BasicStringView<TCharType>::starts_with(TCharType cChar) const {
  return starts_with(BasicStringView(&cChar, 1));
}

template<typename TCharType>
bool BasicStringView<TCharType>::starts_with(const TCharType *pStr) const {
  return starts_with(BasicStringView(pStr));
}

template<typename TCharType>
bool BasicStringView<TCharType>::ends_with(BasicStringView cSource) const {
  return (size() >= cSource.size()) && (compare(size() - cSource.size(), NPOS, cSource) == 0);
}

template<typename TCharType>
bool BasicStringView<TCharType>::ends_with(TCharType cChar) const {
  return ends_with(BasicStringView(&cChar, 1));
}

template<typename TCharType>
bool BasicStringView<TCharType>::ends_with(const TCharType *pStr) const {
  return ends_with(BasicStringView(pStr));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]