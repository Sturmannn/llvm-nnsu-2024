set(Plugin_Name "durandin_vladimir_inline_pass_plugin")
if (NOT WIN32 AND NOT CYGWIN)
    
    file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.hpp)
    add_llvm_library( ${Plugin_Name} MODULE ${ALL_SOURCE_FILES}
      PLUGIN_TOOL
      opt
      )
    set(LLVM_TEST_DEPENDS ${Plugin_Name} ${LLVM_TEST_DEPENDS} PARENT_SCOPE)

    message(STATUS "-- Plugin: ${Plugin_Name} -- BUILDED")
else()
    message(STATUS "-- Plugin: ${Plugin_Name} -- NOT BUILDED")
endif()