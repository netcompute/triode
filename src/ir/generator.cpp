//
// Created by gd on 2018/10/16.
//
#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Pass.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/Support/raw_ostream.h>
#include "generator.h"

namespace triode {
namespace ir {

void Generator::Generate(NBlock &root) {
  llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);

  llvm::Function *dropFunc = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, "drop", module);

  root.GenerateIR(*this);


  llvm::legacy::PassManager pm;
  pm.add(llvm::createPrintModulePass(llvm::outs()));
  pm.run(*module);
//  module->dump();
}

}
}