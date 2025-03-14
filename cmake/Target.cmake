#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2025 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////


function(re_include_cmake_file_list file)
  #message(${file})
  include(${file})

  get_filename_component(file_path "${file}" PATH)
  if (file_path)
    list(TRANSFORM FILES PREPEND ${file_path}/)
  endif()

  foreach(f ${FILES})
    get_filename_component(absolute_path ${f} ABSOLUTE)
    if (NOT EXISTS ${absolute_path})
      message(SEND_ERROR "The file ${absolute_path} referenced in ${file} does not exist!")
    endif()
  endforeach()
  # Append the _files.cmake file to the list of files so that it is shown in the IDE
  list(APPEND FILES ${file})

  set(RE_CURRENT_SOURCES ${RE_CURRENT_SOURCES} ${FILES} PARENT_SCOPE)
endfunction()

function(re_add_target)
  unset(RE_CURRENT_SOURCES)

  # Arguments
  set(flags STATIC SHARED MODULE PLUGIN_STATIC PLUGIN_MODULE HEADERONLY EXECUTABLE APPLICATION IMPORTED AUTOMOC AUTOUIC AUTORCC NO_UNITY REFLECT INSTALL)
  set(oneValueArgs NAME NAMESPACE OUTPUT_SUBDIRECTORY OUTPUT_NAME)
  set(multiValueArgs FILES_CMAKE CONDITIONAL_INCLUDES GENERATED_FILES INCLUDE_DIRECTORIES COMPILE_DEFINITIONS BUILD_DEPENDENCIES RUNTIME_DEPENDENCIES EXTERNAL_DEPENDENCIES PLATFORM_INCLUDE_FILES TARGET_PROPERTIES AUTOGEN_RULES)

  # Parse arguments
  cmake_parse_arguments(re_add_target "${flags}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  #message("\n\n\n STARTING ${re_add_target_NAME}")
  # Configure project
  if (NOT re_add_target_NAME)
    message(FATAL_ERROR "You must provide a name for the target!")
  endif()

  # If the PLUGIN_MODULE tag is passed in set the normal MODULE argument
  if (re_add_target_PLUGIN_MODULE)
    set(re_add_target_MODULE ${re_add_target_PLUGIN_MODULE})
  endif()
  # If the PLUGIN_STATIC tag is passed in set the normal STATIC argument
  if (re_add_target_PLUGIN_STATIC)
    set(re_add_target_STATIC ${re_add_target_PLUGIN_STATIC})
  endif()

  # Add conditional includes
  foreach(cond_include in ${re_add_target_CONDITIONAL_INCLUDES})

  endforeach()

  # Fetch all source files
  #re_collection_source_files()
  foreach(f ${re_add_target_FILES_CMAKE})
    re_include_cmake_file_list(${f})
  endforeach()

  # Configure
  # If the PLUGIN_MODULE tag is passed set the normal MODULE argument
  if(re_add_target_PLUGIN_MODULE)
    set(re_add_target_MODULE ${re_add_target_PLUGIN_MODULE})
  endif()
  # If the PLUGIN_STATIC tag is passed mark the target as STATIC
  if(re_add_target_PLUGIN_STATIC)
    set(re_add_target_STATIC ${re_add_target_PLUGIN_STATIC})
  endif()
  unset(linking_options)
  unset(linking_count)
  unset(target_type_options)
  if(re_add_target_STATIC)
    set(linking_options STATIC)
    set(target_type_options STATIC)
    set(linking_count "${linking_count}1")
  endif()
  if(re_add_target_SHARED)
    set(linking_options SHARED)
    set(target_type_options SHARED)
    set(linking_count "${linking_count}1")
  endif()
  if(re_add_target_MODULE)
    set(linking_options ${PAL_LINKOPTION_MODULE})
    set(target_type_options ${PAL_LINKOPTION_MODULE})
    set(linking_count "${linking_count}1")
  endif()
  if(re_add_target_HEADERONLY)
    set(linking_options INTERFACE)
    set(target_type_options INTERFACE)
    set(linking_count "${linking_count}1")
  endif()
  if(re_add_target_EXECUTABLE)
    set(linking_options EXECUTABLE)
    set(linking_count "${linking_count}1")
  endif()
  if(re_add_target_APPLICATION)
    set(linking_options APPLICATION)
    set(linking_count "${linking_count}1")
  endif()
  if(NOT ("${linking_count}" STREQUAL "1"))
    message(FATAL_ERROR "More than one of the following options [STATIC | SHARED | MODULE | HEADERONLY | EXECUTABLE | APPLICATION ] was specified and they are mutually exclusive")
  endif()
  if(re_add_target_IMPORTED)
    list(APPEND target_type_options IMPORTED GLOBAL)
  endif()

  if(re_add_target_NAMESPACE)
    set(interface_name "${re_add_target_NAMESPACE}::${re_add_target_NAME}")
  else()
    set(interface_name "${re_add_target_NAME}")
  endif()

  set(project_NAME ${re_add_target_NAME})
  if(re_add_target_EXECUTABLE)
    add_executable(${re_add_target_NAME}
      ${target_type_options}
      ${RE_CURRENT_SOURCES} ${re_add_target_GENERATED_FILES}
      )
    if(re_add_target_IMPORTED)
      set_target_properties(${re_add_target_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    endif()
  elseif(re_add_target_APPLICATION)
    add_executable(${re_add_target_NAME}
      ${target_type_options}
      ${PAL_EXECUTABLE_APPLICATION_FLAG}
      ${RE_CURRENT_SOURCES} ${re_add_target_GENERATED_FILES}
      )
    if(re_add_target_IMPORTED)
      set_target_properties(${re_add_target_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    endif()
  elseif(re_add_target_HEADERONLY)
    add_library(${re_add_target_NAME}
      ${target_type_options}
      ${RE_CURRENT_SOURCES} ${re_add_target_GENERATED_FILES}
      )
  else()
    add_library(${re_add_target_NAME}
      ${target_type_options}
      ${RE_CURRENT_SOURCES} ${re_add_target_GENERATED_FILES}
      )
  endif()

  if(${re_add_target_EXECUTABLE} OR ${re_add_target_APPLICATION})
    add_executable(${interface_name} ALIAS ${re_add_target_NAME})
  else()
    add_library(${interface_name} ALIAS ${re_add_target_NAME})
  endif()

  if(re_add_target_PLUGIN_MODULE OR re_add_target_PLUGIN_STATIC)
    set_target_properties(${re_add_target_NAME} PROPERTIES PLUGIN_MODULE TRUE)
  endif()

  if (re_add_target_INCLUDE_DIRECTORIES)
    target_include_directories(${re_add_target_NAME}
      ${re_add_target_INCLUDE_DIRECTORIES}
      )
  endif()

  # Add the build dependencies
  if (re_add_target_BUILD_DEPENDENCIES)
    #message("re_add_target_BUILD_DEPENDENCIES: ${re_add_target_BUILD_DEPENDENCIES}")
    re_search_for_external_dependencies("${re_add_target_BUILD_DEPENDENCIES}")
    foreach (dep "${re_add_target_BUILD_DEPENDENCIES}")
      unset(target_group)
      foreach(d ${dep})
        if (NOT d STREQUAL "PUBLIC" AND NOT d STREQUAL "PRIVATE")
          #message(INFO ${d})

          target_include_directories(${re_add_target_NAME} ${target_group} ${${d}_INCLUDE_DIRS})
          target_include_directories(${re_add_target_NAME} ${target_group} ${${d}_EXT_INCLUDE_DIRS})
          
          #target_link_libraries(${re_add_target_NAME} ${target_group} ${${d}_LIBRARIES})
          target_link_libraries(${re_add_target_NAME} ${target_group} ${d})
          #message("INFO>> $<TARGET_PROPERTY:${d},INTERFACE_LINK_LIBRARIES>")
          #target_link_libraries(${re_add_target_NAME} PUBLIC $<TARGET_PROPERTY:${d},INTERFACE_LINK_LIBRARIES>)
          target_include_directories(${re_add_target_NAME} ${target_group} $<TARGET_PROPERTY:${d},INCLUDE_DIRECTORIES>)
          target_compile_definitions(${re_add_target_NAME} ${target_group} $<TARGET_PROPERTY:${d},COMPILE_DEFINITIONS>)
          target_compile_options(${re_add_target_NAME} ${target_group} $<TARGET_PROPERTY:${d},COMPILE_OPTIONS>)
          
          add_dependencies(${re_add_target_NAME} ${d})
          #target_include_directories(${re_add_target_NAME} PUBLIC ${RE_CURRENT_EXT_INCLUDE_DIRS})
        else()
          set(target_group ${d})
        endif()
      endforeach()
    endforeach()
    #target_link_libraries(
    #  ${re_add_target_NAME}
    #  ${re_add_target_BUILD_DEPENDENCIES})
  endif()

  # Add the runtime dependencies
  if (re_add_target_RUNTIME_DEPENDENCIES)
    #message("re_add_target_RUNTIME_DEPENDENCIES: ${re_add_target_RUNTIME_DEPENDENCIES}")
    re_search_for_external_dependencies(${re_add_target_NAME} "${re_add_target_RUNTIME_DEPENDENCIES}")
  endif()
  if (re_add_target_EXTERNAL_DEPENDENCIES)

    re_search_for_external_dependencies(${re_add_target_NAME}  "${re_add_target_EXTERNAL_DEPENDENCIES}")
  endif()

  re_configure_target_platform_properties()

  if(re_add_target_COMPILE_DEFINITIONS)
    target_compile_definitions(${re_add_target_NAME}
      ${re_add_target_COMPILE_DEFINITIONS}
      )
  endif()

  if(re_add_target_TARGET_PROPERTIES)
    set_target_properties(${re_add_target_NAME} PROPERTIES COMPILE_FLAGS
      ${re_add_target_TARGET_PROPERTIES})
  endif()

  # Handle Qt MOC, RCC, UIC
  foreach(prop IN ITEMS AUTOMOC AUTORCC)
    if(${re_add_target_${prop}})
      set_property(TARGET ${re_add_target_NAME} PROPERTY ${prop} ON)
    endif()
  endforeach()
  if(${re_add_target_AUTOUIC})
    get_target_property(all_ui_sources ${re_add_target_NAME} SOURCES)
    list(FILTER all_ui_sources INCLUDE REGEX "^.*\\.ui$")
    if(NOT all_ui_sources)
      message(FATAL_ERROR "Target ${re_add_target_NAME} contains AUTOUIC but doesnt have any .ui file")
    endif()
    ly_qt_uic_target(${re_add_target_NAME})
  endif()

  install(
    TARGETS ${re_add_target_NAME}
    RUNTIME DESTINATION ${RE_BIN_DIR}
    LIBRARY DESTINATION ${RE_LIB_DIR}
    ARCHIVE DESTINATION ${RE_LIB_DIR}
  )
  if (re_add_target_INSTALL)
    install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/public/${re_add_target_NAME}
      DESTINATION ${RE_INCLUDE_DIR}
    )
  endif()

endfunction(re_add_target)


macro(re_configure_target_platform_properties)
  foreach(platform_include_file ${re_add_target_PLATFORM_INCLUDE_FILES})
    set(RE_BUILD_DEPENDENCIES)
    set(RE_RUNTIME_DEPENDENCIES)
    set(RE_COMPILE_DEFINITIONS)

    #message("RE_BUILD_DEPENDENCIES: ${RE_BUILD_DEPENDENCIES}")
    #message("RE_RUNTIME_DEPENDENCIES: ${RE_RUNTIME_DEPENDENCIES}")
    #message("platform_include_file: ${platform_include_file}")
    include(${platform_include_file} RESULT_VARIABLE re_platform_cmake_file)
    #message("re_platform_cmake_file: ${re_platform_cmake_file}")
    #message("RE_BUILD_DEPENDENCIES: ${RE_BUILD_DEPENDENCIES}")
    #message("RE_RUNTIME_DEPENDENCIES: ${RE_RUNTIME_DEPENDENCIES}")
    #message("re_add_target_NAME: ${re_add_target_NAME}")

    if(RE_BUILD_DEPENDENCIES)
      foreach(item ${RE_BUILD_DEPENDENCIES})
        if (re_add_target_HEADERONLY)
          target_link_libraries(${re_add_target_NAME} ${linking_options} ${item})
        else()
          target_link_libraries(${re_add_target_NAME} PRIVATE ${item})
        endif()
      endforeach()
    endif()
    if(RE_RUNTIME_DEPENDENCIES)
      foreach(item ${RE_RUNTIME_DEPENDENCIES})
        #target_link_libraries(${re_add_target_NAME}  ${item})
      endforeach()
    endif()

  endforeach()
endmacro()


macro(re_search_for_external_dependencies TARGET)
  #message("re_search_for_external_dependencies: ${ARGN}")
  foreach(dependency ${ARGN})
    string(REPLACE "::" ";" dependency_list ${dependency})
    list(GET dependency_list 0 dep_namespace)
    if (${dep_namespace} STREQUAL "External")
      list(GET dependency_list 1 asdasd)
      if (NOT TARGET ${dependency})
        list(GET dependency_list 1 pkg)
        list(LENGTH dependency_list dependency_list_length)
        if (dependency_list_length GREATER 2)
          # Interface specifier
          list(GET dependency_list 2 component)
          list(APPEND packages_with_components ${pkg})
          list(APPEND ${pkg}_components ${component})
        else()
          find_package(${pkg} REQUIRED MODULE)
          target_include_directories(${TARGET} PUBLIC ${${pkg}_INCLUDE_DIR})
          target_link_libraries(${TARGET} PUBLIC ${${pkg}_LIBRARY})
        endif()
      else()
        list(GET dependency_list 1 pkg)
        find_package(${pkg} REQUIRED MODULE)

        target_include_directories(${TARGET} PUBLIC ${${pkg}_INCLUDE_DIR})
        target_link_libraries(${TARGET} PUBLIC ${${pkg}_LIBRARY})
        target_link_libraries(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${pkg},INTERFACE_LINK_LIBRARIES>)
        target_include_directories(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${pkg},INTERFACE_INCLUDE_DIRECTORIES>)
        target_compile_definitions(${TARGET} ${target_group} $<TARGET_PROPERTY:External::${pkg},INTERFACE_COMPILE_DEFINITIONS>)
        target_compile_options(${TARGET} ${target_group} $<TARGET_PROPERTY:External::${pkg},INTERFACE_COMPILE_OPTIONS>)
      endif()
    endif()
  endforeach()

  # Add externals with components
  foreach(dep IN LISTS packages_with_components)
    find_package(${dep} REQUIRED MODULE COMPONENTS ${${dep}_components})

    #target_link_libraries(${TARGET} PUBLIC "External::${dep}::${${dep}_components}")
    target_link_libraries(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${dep}::${${dep}_components},INTERFACE_LINK_LIBRARIES>)
    target_include_directories(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${dep}::${${dep}_components},INTERFACE_INCLUDE_DIRECTORIES>)

  endforeach()
endmacro()