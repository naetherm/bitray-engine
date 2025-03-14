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
#include "engine/engine/engine_core.h"
#include <core/core/server_impl.h>
#include <core/config/config_server.h>
#include <core/io/io_server.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace engine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
EngineCore::EngineCore() {
  register_core_services();
}

EngineCore::~EngineCore() {
  clear();
}


void EngineCore::clear() {
  for (auto* server: mServersList) {
    delete server;
    server = nullptr;
  }

  mServersList.clear();
}


core::uint32 EngineCore::get_number_of_servers() const {
  return mServersList.size();
}

const core::Vector<core::ServerImpl*>& EngineCore::get_all_servers() const {
  return mServersList;
}

const core::ServerImpl* EngineCore::get_server_by_index(core::uint32 index) const {
  return mServersList[index];
}

const core::ServerImpl* EngineCore::get_server_by_name(const core::String& name) const {
  return mServersMap[name];
}

bool EngineCore::has_server_with_name(const core::String& name) const {
  return mServersMap.find(name) != mServersMap.end();
}

void EngineCore::register_server(core::ServerImpl* server, const core::String& name) {
  if (mServersMap.find(name) != mServersMap.end()) {
    mServersList.push_back(server);
    mServersMap[name] = server;
  }
}

void EngineCore::register_core_services() {
  // Register core services
  register_server(new core::ConfigServer(), BE_CONFIG_SERVER_NAME);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}