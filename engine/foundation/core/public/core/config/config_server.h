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
class Config;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class ConfigServer : public ServerImpl {
public:
  /**
   * @brief
   * Constructor.
   */
  ConfigServer();

  /**
   * @brief
   * Destructor.
   */
  ~ConfigServer() override;


  /**
   * @brief
   * Reads the config from the given file path.
   *
   * @param[in] configFilename
   * The config file name.
   *
   * @return
   * 'true' if everything went fine, 'false' otherwise.
   */
  [[nodiscard]] bool read(const String& configFilename);

  /**
   * @brief
   * Writes the config back to the given file path.
   *
   * @param[in] configFilename
   * The config file name.
   *
   * @return
   * 'true' if everything went fine, 'false' otherwise.
   */
  [[nodiscard]] bool write(const String& configFilename) const;


  /**
   * @brief
   * Returns the config representation.
   *
   * @return
   * Config representation.
   */
  [[nodiscard]] const Config& get_config() const;

  /**
   * @brief
   * Returns the config representation.
   *
   * @return
   * Config representation.
   */
  [[nodiscard]] Config& get_config();

private:
  /** Pointer to single config implementation */
 Config* mConfig;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}