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
#include "core/core/server_impl.h"
#include "core/container/hash_map.h"
#include "core/container/vector.h"
#include "core/string/string.h"


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
class ClassTypeInfo;
class EnumTypeInfo;
class PrimitiveTypeInfo;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * RttiTypeServer
 *
 * @brief
 * Central location for RTTI type registration.
 *
 * This class is the central location for registering RTTI types. It provides
 * methods for registering primitive types, class types and enum types.
 */
class RttiTypeServer : public ServerImpl {
public:

  /**
 * @brief
 * Retrieves the singleton instance of the RttiTypeServer.
 *
 * @return A reference to the singleton RttiTypeServer instance.
 */
static RttiTypeServer& instance();

public:

  /**
   * @brief
   * Default constructor for the RttiTypeServer class.
   *
   * This constructor is protected to ensure that the singleton instance of the
   * RttiTypeServer is created via the instance() method.
   */
  RttiTypeServer();

  /**
   * @brief
   * Destructor for the RttiTypeServer class.
   *
   * This destructor cleans up any resources used by the RttiTypeServer.
   */
  ~RttiTypeServer() override;

public:

  /**
   * @brief
   * Registers a primitive type with the RttiTypeServer.
   *
   * @param name
   * The name of the primitive type to register.
   * @param primitiveTypeInfo
   * A pointer to the PrimitiveTypeInfo object representing the primitive type.
   */
  void register_primitive_type(const String& name, PrimitiveTypeInfo* primitiveTypeInfo);

  /**
   * @brief
   * Retrieves the PrimitiveTypeInfo object associated with the specified
   * primitive type name.
   *
   * @param name
   * The name of the primitive type.
   *
   * @return A pointer to the PrimitiveTypeInfo object associated with the
   * specified primitive type name, or nullptr if the primitive type is not
   * registered with the RttiTypeServer.
   */
  PrimitiveTypeInfo* get_primitive_type(const String& name);


  /**
   * @brief
   * Registers a class type with the RttiTypeServer.
   *
   * @param name
   * The name of the class type to register.
   * @param classTypeInfo
   * A pointer to the ClassTypeInfo object representing the class type.
   */
  void register_class_type(const String& name, ClassTypeInfo* classTypeInfo);

  /**
   * @brief
   * Retrieves the ClassTypeInfo object associated with the specified class type name.
   *
   * @param name
   * The name of the class type.
   *
   * @return A pointer to the ClassTypeInfo object associated with the specified
   * class type name, or nullptr if the class type is not registered with the
   * RttiTypeServer.
   */
  ClassTypeInfo* get_class_type(const String& name);


  /**
   * @brief
   * Registers an enum type with the RttiTypeServer.
   *
   * @param name
   * The name of the enum type to register.
   * @param enumTypeInfo
   * A pointer to the EnumTypeInfo object representing the enum type.
   */
  void register_enum_type(const String& name, EnumTypeInfo* enumTypeInfo);

  /**
   * @brief
   * Retrieves the EnumTypeInfo object associated with the specified enum type name.
   *
   * @param name
   * The name of the enum type.
   *
   * @return A pointer to the EnumTypeInfo object associated with the specified
   * enum type name, or nullptr if the enum type is not registered with the
   * RttiTypeServer.
   */
  EnumTypeInfo* get_enum_type(const String& name);

private:

  /** Maps primitive type names to PrimitiveTypeInfo objects. */
  core::hash_map<String, PrimitiveTypeInfo*> mPrimitiveTypesMap;
  /** Maps class type names to ClassTypeInfo objects. */
  core::hash_map<String, ClassTypeInfo*> mClassTypeMap;
  /** Maps enum type names to EnumTypeInfo objects. */
  core::hash_map<String, EnumTypeInfo*> mEnumTypeMap;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}