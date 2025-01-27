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
#include "core/rtti/type/enum_type.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class NullType {
};

template <typename T>
class InvalidType {
};


//[-------------------------------------------------------]
//[ Helper classes                                        ]
//[-------------------------------------------------------]
class TrueType {
};

class FalseType {
};

struct ConversionHelper
{
  template <typename T> ConversionHelper(const volatile T&);
  template <typename T> ConversionHelper(T&);
};


//[-------------------------------------------------------]
//[ Generic templates                                     ]
//[-------------------------------------------------------]
// Implementation for T1 != T2
template <typename T1, typename T2, template <typename T> class TMPL, typename TYPE>
class SpecializeIfEqual {
public:
  typedef FalseType Type;
};

// Implementation for T1 == T2
template <typename T1, template <typename T> class TMPL, typename TYPE>
class SpecializeIfEqual<T1, T1, TMPL, TYPE> {
public:
  typedef TMPL<TYPE> Type;
};

template <bool CHECK, typename TYPE, typename REST>
struct ChooseType {
};

// Implementation for CHECK == true
template <typename TYPE, typename REST>
struct ChooseType<true, TYPE, REST> {
  typedef TYPE Type;
};

// Implementation for CHECK == false
template <typename TYPE, typename REST>
struct ChooseType<false, TYPE, REST> {
  typedef typename REST::Type Type;
};

template <typename T1, typename T2>
struct BooleanOr {
  static const bool Value = (T1::Value || T2::Value);
};

template <typename T1, typename T2>
struct BooleanAnd {
  static const bool Value = (T1::Value && T2::Value);
};

template <typename T>
struct BooleanNot {
  static const bool Value = (!T::Value);
};

template <typename T>
class IntegralType {
public:
  enum { IsIntegral = 0 };
  typedef NullType Type;
};

template <>
class IntegralType<uint8> {
public:
  enum { IsIntegral = 1 };
  typedef uint8 Type;
};

template <>
class IntegralType<uint16> {
public:
  enum { IsIntegral = 1 };
  typedef uint16 Type;
};

template <>
class IntegralType<uint32> {
public:
  enum { IsIntegral = 1 };
  typedef uint32 Type;
};

template <>
class IntegralType<int8> {
public:
  enum { IsIntegral = 1 };
  typedef int8 Type;
};

template <>
class IntegralType<int16> {
public:
  enum { IsIntegral = 1 };
  typedef int16 Type;
};

template <>
class IntegralType<int32> {
public:
  enum { IsIntegral = 1 };
  typedef int32 Type;
};

// Implementation for T1 != T2
template<typename T1, typename T2>
struct IsEqual
{
  static const bool Value = false;
};

// Implementation for T1 == T2
template<typename T>
struct IsEqual<T, T>
{
  static const bool Value = true;
};

template<typename From, typename To>
struct IsConvertible
{
private:
  typedef char                          one_byte;
  typedef struct { char two_chars[2]; } two_bytes;

private:
  static one_byte  test(const To&, int);
  static two_bytes test(ConversionHelper, ...);

public:
  static const bool Value = sizeof(test(*(static_cast<From*>(nullptr)), 0)) == 1;


};

template<typename Class, typename Base>
struct IsBaseClass
{
  static const bool Value = BooleanAnd< IsConvertible<Class, Base>, BooleanNot< IsEqual<Class, Base> > >::Value;
};

template<typename T>
struct IsEnum
{
  static const bool Value = IsConvertible<T, int>::Value;
};

template<typename T>
struct IsEnum< InvalidType<T> >
{
  static const bool Value = false;
};

template <typename T>
struct TypeClassification {
  static const bool Value = IsEnum<T>::Value;	/**< 'true' if T is an enum */
};

template <typename T>
struct CheckType {
  typedef typename ChooseType< TypeClassification<T>::Value, core::EnumTypePlain<T>,
            ChooseType< true, InvalidType<T>, InvalidType<T> >
           >::Type Type;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}