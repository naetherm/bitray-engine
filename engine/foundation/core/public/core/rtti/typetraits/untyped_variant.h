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
#include "core/rtti/func/destructor.h"
#include "core/rtti/typetraits/type_storage.h"


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
template <int STORAGE_SIZE = sizeof(int*)>
class UntypedVariant {
public:
	/**
	 * @brief
	 * Creates an empty untyped variant.
	 *
	 * This constructor creates an empty untyped variant.
	 *
	 * @see UntypedVariant(const T &)
	 */
	UntypedVariant() {}

	/**
	 * @brief
	 * Creates an untyped variant from a value of a specific type.
	 *
	 * This constructor creates an untyped variant from a value of a specific type.
	 *
	 * @param cVal The value of the specific type to store in the untyped variant.
	 * @param bAutoDestroy If true, the type's destructor will be called when the untyped variant is destroyed.
	 *
	 * @see UntypedVariant()
	 */
	template <typename T>
	UntypedVariant(T cVal, bool bAutoDestroy = false)
	{
		typedef typename TypeStorage<T>::StorageType Type;
		AccessHelper<T, IsBig<Type>::Value>::Set(cVal, m_pData);

		if (bAutoDestroy)
		{
			m_cDestructor = Destructor::create<Type, IsBig<Type>::Value>();
		}
	}

	/**
	 * @brief
	 * Destructs the untyped variant.
	 *
	 * This destructor calls the `destroy` function, which will call the destructor of the stored type
	 * if the `bAutoDestroy` parameter was set during construction.
	 *
	 * @see destroy()
	 */
	~UntypedVariant()
	{
		destroy();
	}

	//template <typename T>
	//UntypedVariant(const T &&cVal)
	//{
	//	AccessHelper<T, IsBig<T>::Value>::Set(static_cast<T&&>(cVal), m_pData);
	//}

	/**
	 * @brief
	 * Sets the value of the untyped variant.
	 *
	 * This function sets the value of the untyped variant to the given value of the specific type.
	 * If the `bAutoDestroy` parameter is set to true, the type's destructor will be called when the untyped variant is destroyed.
	 *
	 * @param cVal The value of the specific type to store in the untyped variant.
	 * @param bAutoDestroy If true, the type's destructor will be called when the untyped variant is destroyed.
	 *
	 * @see UntypedVariant(const T &), destroy()
	 */
	template <typename T>
	void set(T cVal, bool bAutoDestroy = false)
	{
		destroy();

		typedef typename TypeStorage<T>::StorageType Type;
		AccessHelper<T, IsBig<Type>::Value>::set(cVal, m_pData);

		if (bAutoDestroy)
		{
			m_cDestructor = Destructor::create<Type, IsBig<Type>::Value>();
		}
	}

	/**
	 * @brief
	 * Retrieves the value stored in the untyped variant.
	 *
	 * This function returns the value stored in the untyped variant, cast to the specified type `T`.
	 * The function is marked as `const`, ensuring it does not modify the state of the object.
	 *
	 * @tparam T The type to which the stored value should be cast and returned.
	 *
	 * @return The value of the untyped variant as the specified type `T`.
	 */
	template <typename T>
	const T get() const
	{
		typedef typename TypeStorage<T>::StorageType Type;
		return AccessHelper<T, IsBig<Type>::Value>::get(m_pData);
	}

	/**
	 * @brief
	 * Retrieves the value stored in the untyped variant.
	 *
	 * This function returns the value stored in the untyped variant, cast to the specified type `T`.
	 * The function is not marked as `const`, meaning it may modify the state of the object.
	 *
	 * @tparam T The type to which the stored value should be cast and returned.
	 *
	 * @return The value of the untyped variant as the specified type `T`.
	 */
	template <typename T>
	T get()
	{
		typedef typename TypeStorage<T>::StorageType Type;
		return AccessHelper<T, IsBig<Type>::Value>::get(m_pData);
	}

	template <typename T>
	const T &get_ref() const
	{
		typedef typename TypeStorage<T>::StorageType Type;
		return AccessHelper<T, IsBig<Type>::Value>::get_ref(m_pData);
	}

	template <typename T>
	T &get_ref()
	{
		typedef typename TypeStorage<T>::StorageType Type;
		return AccessHelper<T, IsBig<Type>::Value>::get_ref(m_pData);
	}

	/**
	 * @brief
	 * Destroys the untyped variant.
	 *
	 * This function calls the destructor of the stored type if the `bAutoDestroy` parameter was set during construction.
	 * After calling the destructor, the stored type is reset.
	 *
	 * @see UntypedVariant(const T &), ~UntypedVariant()
	 */
	void destroy()
	{
		m_cDestructor.destroy(m_pData);

		// Reset the destructor!
		m_cDestructor = Destructor();
	}

	/**
	 * @brief
	 * Compares two untyped variants for equality.
	 *
	 * This function currently always returns false, as the untyped variant does not have enough information to compare two arbitrary types.
	 * In the future, this function should be implemented to compare two arbitrary types.
	 *
	 * @param cOther The other untyped variant to compare.
	 *
	 * @return False, always.
	 */
	bool operator==(const UntypedVariant &cOther) const
	{
		return false;
	}

private:
	template <typename T>
	struct IsBig {

		static const bool Value = sizeof(T) > STORAGE_SIZE;
	};

	template <typename T, bool IS_BIG>
	struct AccessHelper;

	template <typename T>
	struct AccessHelper<T, false> {

		typedef TypeStorage<T> Storage;

		/**
		 * @brief
		 * Sets the value of the untyped variant.
		 *
		 * This function sets the value of the untyped variant to the given value of the specific type.
		 * It does not call the destructor of the stored type, so it is not safe to call this function on a live untyped variant.
		 *
		 * @param cVal The value of the specific type to store in the untyped variant.
		 * @param pBuffer A pointer to the memory to store the value in.
		 *
		 * @warning
		 * This function does not check if the given type fits into the storage buffer. If the type is too large, this function will invoke undefined behavior.
		 *
		 * @warning
		 * This function does not support arrays as a type, as the necessary information to store an array is not available.
		 */
		static void set(T cVal, unsigned char *pBuffer)
		{
			// [TODO] Arrays are not supported
			typedef typename Storage::StorageType Type;
			new (pBuffer) Type(Storage::store(cVal));
		}

		/**
		 * @brief
		 * Retrieves the stored value from the buffer.
		 *
		 * This function reads the stored value from the provided buffer
		 * and converts it back to the original type using the `restore` function.
		 *
		 * @param pBuffer A pointer to the memory buffer containing the stored value.
		 *
		 * @return The value of the original type retrieved from the buffer.
		 */
		static T get(unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore(*((Type*)pBuffer));
		}

		/**
		 * @brief
		 * Retrieves the stored value from the buffer.
		 *
		 * This function reads the stored value from the provided buffer
		 * and converts it back to the original type using the `restore` function.
		 *
		 * @param pBuffer A pointer to the memory buffer containing the stored value.
		 *
		 * @return The value of the original type retrieved from the buffer.
		 */
		static const T get(const unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore(*((const Type*)pBuffer));
		}

		static T &get_ref(unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore_ref(*((Type*)pBuffer));
		}

		static const T &get_ref(const unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore_ref(*((const Type*)pBuffer));
		}
	};

	template <typename T>
	struct AccessHelper<T, true> {

		typedef TypeStorage<T> Storage;

		/**
		 * @brief
		 * Sets the value of the untyped variant.
		 *
		 * This function stores the given value of the specific type into the untyped variant.
		 * It allocates memory for the value and stores a pointer to it in the provided buffer.
		 *
		 * @param cVal The value of the specific type to store in the untyped variant.
		 * @param pBuffer A pointer to the memory buffer where the pointer to the stored value will be placed.
		 *
		 * @warning
		 * This function does not support arrays as a type. The necessary information to store an array is not available.
		 */
		static void set(const T &cVal, unsigned char *pBuffer)
		{
			// [TODO] Arrays are not supported
			typedef typename Storage::StorageType Type;
			*((Type**)pBuffer) = new Type(Storage::store(cVal));
		}

		/**
		 * @brief
		 * Retrieves the stored value from the buffer.
		 *
		 * This function reads the stored value from the provided buffer
		 * and converts it back to the original type using the `restore` function.
		 *
		 * @param pBuffer A pointer to the memory buffer containing the stored value.
		 *
		 * @return The value of the original type retrieved from the buffer.
		 */
		static T get(unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore(**((Type**)pBuffer));
		}

		/**
		 * @brief
		 * Retrieves the stored value from the buffer.
		 *
		 * This function reads the stored value from the provided buffer
		 * and converts it back to the original type using the `restore` function.
		 *
		 * @param pBuffer A pointer to the memory buffer containing the stored value.
		 *
		 * @return The value of the original type retrieved from the buffer.
		 */
		static const T get(const unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore(**((const Type**)pBuffer));
		}

		static T &get_ref(unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore_ref(**((Type**)pBuffer));
		}

		static const T &get_ref(const unsigned char *pBuffer)
		{
			typedef typename Storage::StorageType Type;
			return Storage::restore_ref(**((const Type**)pBuffer));
		}
	};

private:
	/** Untyped data buffer */
	unsigned char	m_pData[STORAGE_SIZE];
	/** Untyped destructor*/
  Destructor		m_cDestructor;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/rtti/typetraits/untyped_variant.inl"