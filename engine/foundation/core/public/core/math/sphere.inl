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
//[ Includes                                              ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
Sphere<TType>::Sphere() {
}

template<typename TType>
Sphere<TType>::Sphere(const Sphere& other)
: mRadius(other.mRadius)
, mCenterPosition(other.mCenterPosition) {

}

template<typename TType>
Sphere<TType>::~Sphere() {

}


template<typename TType>
Sphere<TType>& Sphere<TType>::operator=(const Sphere& other) {
  mRadius = other.mRadius;
  mCenterPosition = other.mCenterPosition;
  return *this;
}

template<typename TType>
bool Sphere<TType>::operator==(const Sphere& other) const {
  return mRadius == other.mRadius && mCenterPosition == other.mCenterPosition;
}

template<typename TType>
bool Sphere<TType>::operator!=(const Sphere& other) const {
  return !(*this == other);
}

template<typename TType>
TType Sphere<TType>::get_radius() const {
  return mRadius;
}

template<typename TType>
void Sphere<TType>::set_radius(TType radius) {
  mRadius = radius;
}

template<typename TType>
const Vec3<TType>& Sphere<TType>::get_position() const {
  return mCenterPosition;
}

template<typename TType>
void Sphere<TType>::set_position(const Vec3<TType>& position) {
  mCenterPosition = position;
}

template<typename TType>
void Sphere<TType>::set_position(TType x, TType y, TType z) {
  mCenterPosition = Vec3<TType>(x, y, z);
}





//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}