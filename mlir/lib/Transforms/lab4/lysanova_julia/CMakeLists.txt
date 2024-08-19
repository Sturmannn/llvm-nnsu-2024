set(PluginName LysanovaMathFMA_MLIR_PASS)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
add_llvm_pass_plugin(${PluginName}
        ${ALL_SOURCE_FILES}
        DEPENDS
        intrinsics_gen
        MLIRBuiltinLocationAttributesIncGen
        BUILDTREE_ONLY
        )

set(MLIR_TEST_DEPENDS ${PluginName} ${MLIR_TEST_DEPENDS} PARENT_SCOPE)

message(STATUS "Plugin: ${PluginName} -- BUILT")