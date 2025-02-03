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
#include "core/rtti/type_info/type_info.h"
#include "core/rtti/type_info/class_type_info.h"
#include "core/rtti/type_info/static_type_info.h"
#include "core/std/typetraits/remove_const.h"
#include "core/std/typetraits/remove_reference.h"


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
template <typename T, bool Copyable>
struct Access;

template <typename T>
struct Access<T, true> {

	typedef typename remove_const<typename remove_reference<T>::type>::type NakedType;

	static T get(UntypedVariant<> &cValue, const TypeInfo *pMyTypeInfo) {
		// If the requested type is a const reference of the actually stored type, we can convert it
		TypeInfo *pRequestedTypeInfo = StaticTypeInfo<T>::Get();
		if (pRequestedTypeInfo->get_type_info_type() == TypeInfoType::ReferenceType &&
			((ReferenceTypeInfo*)pRequestedTypeInfo)->is_const() &&
			*((ReferenceTypeInfo*)pRequestedTypeInfo)->get_reference_type() == *pMyTypeInfo) {
			return cValue.get_ref<NakedType>();
		}

		// If the stored type is a const reference to the requested type, we can convert it as well
		if (pMyTypeInfo->get_type_info_type() == TypeInfoType::ReferenceType &&
			((ReferenceTypeInfo*)pMyTypeInfo)->is_const() &&
			*((ReferenceTypeInfo*)pMyTypeInfo)->get_reference_type() == *pRequestedTypeInfo) {
			const T &ref = cValue.get<const T&>();
			return ref;
		}

		return cValue.get<T>();
	}

	static const T get(const UntypedVariant<> &cValue, const TypeInfo *pMyTypeInfo) {
		// If the requested type is a const reference of the actually stored type, we can convert it
		TypeInfo *pRequestedTypeInfo = StaticTypeInfo<T>::Get();
		if (pRequestedTypeInfo->get_type_info_type() == TypeInfoType::ReferenceType &&
			((ReferenceTypeInfo*)pRequestedTypeInfo)->is_const() &&
			*((ReferenceTypeInfo*)pRequestedTypeInfo)->get_reference_type() == *pMyTypeInfo) {
			return cValue.get_ref<NakedType>();
		}

		// If the stored type is a const reference to the requested type, we can convert it as well
		if (pMyTypeInfo->get_type_info_type() == TypeInfoType::ReferenceType &&
			((ReferenceTypeInfo*)pMyTypeInfo)->is_const() &&
			*((ReferenceTypeInfo*)pMyTypeInfo)->get_reference_type() == *pRequestedTypeInfo) {
			const T &ref = cValue.get<const T&>();
			return ref;
		}

		return cValue.get<T>();
	}
};

template <typename T>
struct Access<T, false> {

	typedef typename remove_const<typename remove_reference<T>::type>::type NakedType;

	static T get(UntypedVariant<> &cValue, const TypeInfo *pMyTypeInfo) {
		return cValue.get<T>();
	}

	static const T get(const UntypedVariant<> &cValue, const TypeInfo *pMyTypeInfo) {
		return cValue.get<T>();
	}
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
DynamicObject::DynamicObject(TType instance) {
  set<TType>(instance);
}

inline bool DynamicObject::operator==(const DynamicObject& other) const {
	return (mTypeInfo == other.mTypeInfo && mUntypedVariant == other.mUntypedVariant);
}

template<typename TType>
void DynamicObject::set(TType instance) {
  mUntypedVariant.set<TType>(instance);
  mTypeInfo = get_static_type_info<TType>();
}

inline const core::UntypedVariant<>& DynamicObject::get_untyped() const {
  return mUntypedVariant;
}

inline const TypeInfo* DynamicObject::get_type_info() const {
  return mTypeInfo;
}

template<typename TType>
const TType DynamicObject::get_as() const {
	typedef typename remove_const<typename remove_reference<TType>::type>::type NakedType;
	return Access<TType, StaticTypeInfo<NakedType>::Copyable>::Get(mUntypedVariant, mTypeInfo);
}

template<typename TType>
TType DynamicObject::get_as() {
	typedef typename remove_const<typename remove_reference<TType>::type>::type NakedType;
	return Access<TType, StaticTypeInfo<NakedType>::Copyable>::Get(mUntypedVariant, mTypeInfo);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}