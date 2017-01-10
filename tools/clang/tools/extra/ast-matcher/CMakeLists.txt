set(LLVM_LINK_COMPONENTS support)

add_clang_executable(ast-matcher
	ASTMatcher.cpp
)

target_link_libraries(ast-matcher
	clangTooling
	clangBasic
	clangASTMatchers
)
