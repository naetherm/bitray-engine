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
#include "core/container/vector.h"
#include "core/rtti/typetraits/type_storage.h"
#include "core/rtti/tools/dynamic_object.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <int I, typename T>
struct TupleElement;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tuple value holder
*/
template <int I, typename TValue>
class TupleLeaf {

	//[-------------------------------------------------------]
	//[ Public types                                          ]
	//[-------------------------------------------------------]
	public:
		typedef TypeStorage<TValue> Storage;
		typedef typename Storage::StorageType StorageType;

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TupleLeaf() : m_cValue() {}

		/**
		*  @brief
		*    Copy constructor
		*/
		TupleLeaf(const TupleLeaf &cLeaf) : m_cValue(cLeaf.Get()) {}

		/**
		*  @brief
		*    Value constructor
		*
		*  @param[in] cVal
		*    The value to initialize the leaf with
		*/
		template <typename T>
		explicit TupleLeaf(T &&cVal) : m_cValue(static_cast<T&&>(cVal)) {}

		/**
		*  @brief
		*    Create the leaf from another one on the same position
		*/
		template <typename T>
		explicit TupleLeaf(const TupleLeaf<I, T> &cLeaf) : m_cValue(cLeaf.Get()) {}

		/**
		*  @brief
		*    The assignment operator
		*/
		template <typename T>
		TupleLeaf &operator=(T &&cVal)
		{
			m_cValue = static_cast<T&&>(cVal);
			return *this;
		}

		/**
		*  @brief
		*    Get this leaf's data
		*/
		StorageType &Get()
		{
			return m_cValue;
		}

		/**
		*  @brief
		*    Get this leaf's data
		*/
		const StorageType &Get() const
		{
			return m_cValue;
		}

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		StorageType m_cValue;		/**< The value held by this leaf */
};

/**
*  @brief
*    Helper to hold tuple's indices
*/
template <int... Indices>
struct TupleIndices {

};

/**
*  @brief
*    Helper to generate the tuple indices
*/
template <int Start, typename TupleIndices, int End>
struct MakeTupleIndicesImpl;

template <int Start, int... Indices, int End>
struct MakeTupleIndicesImpl<Start, TupleIndices<Indices...>, End> {

	typedef typename MakeTupleIndicesImpl<Start + 1, TupleIndices<Indices..., Start>, End>::_Type _Type;
};

template <int End, int... Indices>
struct MakeTupleIndicesImpl<End, TupleIndices<Indices...>, End> {

	typedef TupleIndices<Indices...> _Type;
};

template <int End, int Start = 0>
struct MakeTupleIndices {

	typedef typename MakeTupleIndicesImpl<Start, TupleIndices<>, End>::_Type _Type;
};

/**
*  @brief
*    Tuple core implementation
*
*  @remarks
*    This implements a 'flat' tuple using multiple inheritance
*/
template <typename TIndices, typename... Types>
struct TupleImpl;

template <int... Indices, typename... Types>
struct TupleImpl<TupleIndices<Indices...>, Types...> : public TupleLeaf<Indices, Types>... {

};

/**
*  @brief
*    Holder of a generic sequence of typed values
*/
template <typename... Types>
class Tuple {

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Tuple() {}

		/**
		*  @brief
		*    Value constructor
		*/
//		Tuple(Types... values) { /* [TODO] */ }

	//[-------------------------------------------------------]
	//[ Private types                                         ]
	//[-------------------------------------------------------]
	private:
		typedef TupleImpl<typename MakeTupleIndices<sizeof...(Types)>::_Type, Types...> _Impl;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	public:
		_Impl m_cImpl;
};

/**
*  @brief
*    Tuple element type helper
*/
template <int I, typename Head, typename... Tail>
struct TupleElement<I, Tuple<Head, Tail...>> : TupleElement<I-1, Tuple<Tail...>> {
};

template <typename Head, typename... Tail>
struct TupleElement<0, Tuple<Head, Tail...>> {

	typedef Head Type;
	typedef typename TypeStorage<Type>::StorageType StorageType;
};

/**
*  @brief
*    Get the value on the specified index from the tuple
*/
template <int I, typename... T>
const typename TupleElement<I, Tuple<T...>>::Type TupleGet(const Tuple<T...> &cTuple)
{
	typedef typename TupleElement<I, Tuple<T...>>::Type Type;
	return TypeStorage<Type>::restore(static_cast<const TupleLeaf<I, Type>&>(cTuple.m_cImpl).Get());
}

/**
*  @brief
*    Get the value on the specified index from the tuple
*/
template <int I, typename... T>
typename TupleElement<I, Tuple<T...>>::Type TupleGet(Tuple<T...> &cTuple)
{
	typedef typename TupleElement<I, Tuple<T...>>::Type Type;
	return TypeStorage<Type>::restore(static_cast<TupleLeaf<I, Type>&>(cTuple.m_cImpl).Get());
}

/**
*  @brief
*    Store the value on the specified index from the tuple
*/
template <int I, typename... T>
void TupleStore(Tuple<T...> &cTuple, const typename TupleElement<I, Tuple<T...>>::Type &cValue)
{
	typedef typename TupleElement<I, Tuple<T...>>::Type Type;
	static_cast<TupleLeaf<I, Type>&>(cTuple.m_cImpl).Get() = TypeStorage<Type>::store(cValue);;
}

/**
*  @brief
*    Helper to unroll tuple's indices
*/
template <int... I>
struct IndexSequence {
};

template <int N, int... I>
struct MakeIndexSequence : public MakeIndexSequence<N - 1, N - 1, I...> {
};

template <int... I>
struct MakeIndexSequence<0, I...> : public IndexSequence<I...>{
};

template <int N, typename... T>
struct TupleFromDynamicObjectImpl {

	static void Make(Vector<DynamicObject> &cIterator, Tuple<T...> &cTuple)
	{
		TupleFromDynamicObjectImpl<N - 1, T...>::Make(cIterator, cTuple);

		typedef typename TupleElement<N - 1, Tuple<T...>>::Type OriginalType;
		typedef typename TupleElement<N - 1, Tuple<T...>>::StorageType StorageType;

		if (!cIterator.empty())
		{
			TupleStore<N - 1>(cTuple, cIterator[N-1].get_as<typename TupleElement<N - 1, Tuple<T...>>::Type>());
		}
	}
};

template <typename... T>
struct TupleFromDynamicObjectImpl<0, T...> {

	static void Make(Vector<DynamicObject> &cIterator, Tuple<T...> &cTuple)
	{
		// End of line
	}
};

/**
*  @brief
*    Construct tuple from untyped variant
*/
template <typename... T>
void TupleFromDynamicObject(Vector<DynamicObject> *pDyn, Tuple<T...> &cTuple)
{
	//Vector<DynamicObject> iter = pDyn->GetIterator();
	TupleFromDynamicObjectImpl<sizeof...(T), T...>::Make(*pDyn, cTuple);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
