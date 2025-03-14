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
#include "engine/engine.h"
#include <core/container/map.h>
#include <core/container/vector.h>
#include <core/string/string.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace core {
class ServerImpl;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace engine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class EngineCore {
public:

  EngineCore();

  ~EngineCore();


  void clear();


  [[nodiscard]] core::uint32 get_number_of_servers() const;

  [[nodiscard]] const core::Vector<core::ServerImpl*>& get_all_servers() const;

  [[nodiscard]] const core::ServerImpl* get_server_by_index(core::uint32 index) const;

  [[nodiscard]] const core::ServerImpl* get_server_by_name(const core::String& name) const;

  [[nodiscard]] bool has_server_with_name(const core::String& name) const;


  void register_server(core::ServerImpl* server, const core::String& name);

  template<typename TServer>
  const TServer* get_server(const core::String& name) const;

  template<typename TServer>
  TServer* get_server(const core::String& name);

protected:

  void register_core_services();

private:

  core::Vector<core::ServerImpl*> mServersList;
  core::Map<core::String, core::ServerImpl*> mServersMap;
};




//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "engine/engine/engine_core.inl"