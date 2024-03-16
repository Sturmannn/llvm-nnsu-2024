#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace clang;
using namespace llvm;

class AddAlwaysInlineConsumer : public ASTConsumer {
public:


bool HasCondition(clang::Stmt *S) {
  if (!S) {
    return false;
  }

  // Проверка наличия условий на текущем уровне
  if (isa<IfStmt>(S) || isa<SwitchStmt>(S) || isa<WhileStmt>(S) ||
      isa<DoStmt>(S) || isa<ForStmt>(S)) {
    return true;
  }

  // Рекурсивный поиск условий в дочерних элементах
  for (Stmt *Child : S->children()) {
    if (HasCondition(Child)) {
      return true;
    }
  }

  return false;
}

  bool HandleTopLevelDecl(DeclGroupRef D) override {
    clang::FunctionDecl *FD = nullptr;
    for (Decl *Decl : D)
    {
      if (FD = dyn_cast<FunctionDecl>(Decl)) {
        if (!HasCondition(FD->getBody())) {
          FD->addAttr(clang::AlwaysInlineAttr::CreateImplicit(FD->getASTContext()));
          FD->print(llvm::outs() << "__attribute__((always_inline)) ");
        }
        else FD->print(llvm::outs());
      }
    }
    return true;
  }
};

class AddAlwaysInlineAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &Compiler,
                                                 llvm::StringRef InFile) override {
    return std::make_unique<AddAlwaysInlineConsumer>();
  }
  
  bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
    return true;
  }
};

static clang::FrontendPluginRegistry::Add<AddAlwaysInlineAction> X("AddAlwaysInline",
                                                               "Adds always_inline to functions without conditions");



// #include "clang/Frontend/FrontendPluginRegistry.h"
// #include "clang/Frontend/CompilerInstance.h"
// #include "clang/AST/ASTConsumer.h"
// #include "clang/AST/ASTContext.h"
// #include "clang/AST/RecursiveASTVisitor.h"
// #include "llvm/Support/raw_ostream.h"

// using namespace clang;

// class ASTDumpConsumer : public ASTConsumer {
// public:
//     void HandleTranslationUnit(ASTContext &Context) override {
//         llvm::outs() << "AST Dump:\n";
//         Context.getTranslationUnitDecl()->dump();
//     }
// };

// class ASTDumpAction : public PluginASTAction {
// protected:
//     std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
//         return std::make_unique<ASTDumpConsumer>();
//     }

//     bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
//         return true;
//     }
// };

// static FrontendPluginRegistry::Add<ASTDumpAction>
// X("assst-dump", "Dump the AST of the input code");

