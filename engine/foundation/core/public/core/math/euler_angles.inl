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
#include "core/math/quaternion.h"
#include "core/math/mat33.h"
#include "core/math/mat44.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


static const float FloatEpsilon = 1.192092896e-07f; // Smallest such that 1.0f+FloatEpsilon != 1.0
// EulGetOrd unpacks all useful information about order simultaneously
#define EulSafe "\000\001\002\000"
#define EulNext "\001\002\000\001"
#define EulGetOrd(ord,i,j,k,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];}
// Original code was
// #define EulGetOrd(ord,i,j,k,h,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
//    n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];h=s?k:i;}
// but we don't need "h", so it was removed to spare the compiler the work *g*


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
Quaternion<TType> EulerAngles<TType>::to_quaternion(TType x, TType y, TType z, EOrder order) {
  int i, j, k, n, s, f;
  EulGetOrd(order, i, j, k, n, s, f);
  if (f == 1) {
    const float t = x;
    x = z;
    z = t;
  }
  if (n == 1)
    y = -y;

  const TType ti = x*0.5;
  const TType tj = y*0.5;
  const TType th = z*0.5;
  const TType ci = Math::cos(ti);
  const TType cj = Math::cos(tj);
  const TType ch = Math::cos(th);
  const TType si = Math::sin(ti);
  const TType sj = Math::sin(tj);
  const TType sh = Math::sin(th);
  const TType cc = ci*ch;
  const TType cs = ci*sh;
  const TType sc = si*ch;
  const TType ss = si*sh;

  Quaternion<TType> q;

  TType a[3] = { 0.0, 0.0, 0.0 };
  if (s == 1) {
    a[i] = cj*(cs + sc);	// Could speed up with trig identities
    a[j] = sj*(cc + ss);
    a[k] = sj*(cs - sc);
    q.w = static_cast<float>(cj*(cc - ss));
  } else {
    a[i] = cj*sc - sj*cs;
    a[j] = cj*ss + sj*cc;
    a[k] = cj*cs - sj*sc;
    q.w = static_cast<float>(cj*cc + sj*ss);
  }
  if (n == 1)
    a[j] = -a[j];

  q.x = static_cast<float>(a[0]);
  q.y = static_cast<float>(a[1]);
  q.z = static_cast<float>(a[2]);

  return q;
}

template<typename TType>
Quaternion<TType> EulerAngles<TType>::from_quaternion(Vec3<TType> angles, EOrder order) {
  int i, j, k, n, s, f;
  EulGetOrd(order, i, j, k, n, s, f);
  if (f == 1) {
    const float t = angles.x;
    angles.x = angles.z;
    angles.z = t;
  }
  if (n == 1)
    angles.y = -angles.y;

  const TType ti = angles.x*0.5;
  const TType tj = angles.y*0.5;
  const TType th = angles.z*0.5;
  const TType ci = Math::cos(ti);
  const TType cj = Math::cos(tj);
  const TType ch = Math::cos(th);
  const TType si = Math::sin(ti);
  const TType sj = Math::sin(tj);
  const TType sh = Math::sin(th);
  const TType cc = ci*ch;
  const TType cs = ci*sh;
  const TType sc = si*ch;
  const TType ss = si*sh;

  Quaternion<TType> q;

  TType a[3] = { 0.0, 0.0, 0.0 };
  if (s == 1) {
    a[i] = cj*(cs + sc);	// Could speed up with trig identities
    a[j] = sj*(cc + ss);
    a[k] = sj*(cs - sc);
    q.w = static_cast<float>(cj*(cc - ss));
  } else {
    a[i] = cj*sc - sj*cs;
    a[j] = cj*ss + sj*cc;
    a[k] = cj*cs - sj*sc;
    q.w = static_cast<float>(cj*cc + sj*ss);
  }
  if (n == 1)
    a[j] = -a[j];

  q.x = static_cast<float>(a[0]);
  q.y = static_cast<float>(a[1]);
  q.z = static_cast<float>(a[2]);

  return q;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}