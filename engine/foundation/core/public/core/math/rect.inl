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


template<typename TType>
Rect<TType>::Rect()
: x(0), y(0), w(0), h(0) {

}

template<typename TType>
Rect<TType>::Rect(const Rect &rhs)
: x(rhs.x), y(rhs.y), w(rhs.w), h(rhs.h) {

}

template<typename TType>
Rect<TType>::Rect(TType x, TType y, TType w, TType h)
: x(x), y(y), w(w), h(h) {

}

template<typename TType>
Rect<TType>::~Rect() {

}


template<typename TType>
Rect<TType>& Rect<TType>::operator=(const Rect<TType>& rhs) {
  x = rhs.x;
  y = rhs.y;
  w = rhs.w;
  h = rhs.h;
  return *this;
}


template<typename TType>
bool Rect<TType>::operator==(const Rect &rhs) const {
  return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h;
}

template<typename TType>
bool Rect<TType>::operator!=(const Rect &rhs) const {
  return !operator==(rhs);
}


template<typename TType>
Vec2<TType> Rect<TType>::get_center() const {
  return Vec2<TType>(
    x + w / 2,
    y + h / 2);
}

template<typename TType>
TType Rect<TType>::get_x() const {
  return x;
}

template<typename TType>
TType Rect<TType>::get_y() const {
  return y;
}

template<typename TType>
TType Rect<TType>::get_width() const {
  return w;
}

template<typename TType>
TType Rect<TType>::get_height() const {
  return h;
}

template<typename TType>
TType Rect<TType>::calculate_surface() const {
  return w * h;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace core