#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

class PrintClassFieldsVisitor
    : public clang::RecursiveASTVisitor<PrintClassFieldsVisitor> {
public:
  bool printFields;

  PrintClassFieldsVisitor(bool printFields) : printFields(printFields) {}

bool VisitCXXRecordDecl(clang::CXXRecordDecl *declaration) {
  if (declaration->isClass() || declaration->isStruct()) {
    llvm::outs() << "Class Name: " << declaration->getNameAsString() << "\n";
    if (printFields) {
      for (auto it = declaration->decls_begin(); it != declaration->decls_end(); ++it) {
        if (auto field = llvm::dyn_cast<clang::FieldDecl>(*it)) {
          llvm::outs() << "|_" << field->getNameAsString() << "\n";
        } else if (auto var = llvm::dyn_cast<clang::VarDecl>(*it)) {
          if (var->isStaticDataMember()) {
            llvm::outs() << "|_" << var->getNameAsString() << "\n";
          }
        }
      }
    }
    llvm::outs() << "\n";
  }
  return true;
}
};

class PrintClassFieldsConsumer : public clang::ASTConsumer {
public:
  PrintClassFieldsVisitor Visitor;

  PrintClassFieldsConsumer(bool printFields) : Visitor(printFields) {}

  void HandleTranslationUnit(clang::ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
};

class PrintClassFieldsAction : public clang::PluginASTAction {
public:
  bool printFields = true;

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<PrintClassFieldsConsumer>(printFields);
  }

  bool ParseArgs(const clang::CompilerInstance &CI,
                 const std::vector<std::string> &Args) override {
    for (const auto &arg : Args) {
      if (arg == "no_fields") {
        printFields = false;
      }
    }
    return true;
  }
};

static clang::FrontendPluginRegistry::Add<PrintClassFieldsAction>
    X("prin-elds", "Prints names of all classes and their fields");