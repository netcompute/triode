//
// Created by gd on 2018/10/16.
//

#ifndef TRIODE_GENERATOR_H
#define TRIODE_GENERATOR_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#include "node.h"

namespace triode {
namespace ir {

class Generator {
  llvm::LLVMContext context;
  llvm::Module *module;
  llvm::IRBuilder<> builder;
  std::map<std::string, llvm::StructType*> _structList;
  std::map<std::string, llvm::Value*> _locals;

 public:
  Generator() : builder(context) {
    module = new llvm::Module("main", context);
  }

  llvm::LLVMContext &GetContext() { return context; }
  llvm::Module *GetModule() { return module; }
  llvm::IRBuilder<> &GetBuilder() { return builder; }

  void Generate(triode::NBlock &root);

  void AddStruct(std::string name, llvm::StructType *type) { _structList[name] = type; }
  llvm::Type *GetType(std::string name) { return _structList[name]; }

  llvm::Value *GetLocalValue(std::string name) { return _locals[name]; }
  void SetLocalValue(std::string name, llvm::Value* value) { _locals[name] = value; };
};

}
}

#endif //TRIODE_GENERATOR_H
