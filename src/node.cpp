//
// Created by gd on 2018/10/16.
//
#include <llvm/IR/Function.h>

#include "node.h"
#include "ir/generator.h"

namespace triode {

llvm::Value *NBlock::GenerateIR(triode::ir::Generator &generator) {

  StatementList::const_iterator it;
  for (it = statements.begin(); it != statements.end(); ++it) {
    (**it).GenerateIR(generator);
  }
  return NULL;
}

llvm::Value *NFunctionDeclaration::GenerateIR(triode::ir::Generator &generator) {

  std::vector<llvm::Type *> arg_types;
  for (auto arg: this->arguments) {
    arg_types.push_back(generator.GetType(arg->type.name));
  }

  llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(generator.GetContext()), arg_types, false);

  llvm::Function *function =
      llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, id.name.c_str(), generator.GetModule());

  llvm::BasicBlock *bblock = llvm::BasicBlock::Create(generator.GetContext(), "entry", function, nullptr);

  generator.GetBuilder().SetInsertPoint(bblock); // must add this

  auto origin_arg = this->arguments.begin();

  for (auto &arg_it: function->args()) {

    llvm::Value *argAlloc = (*origin_arg)->GenerateIR(generator);
    generator.SetLocalValue((*origin_arg)->id.name, argAlloc);
    generator.GetBuilder().CreateStore(&arg_it, argAlloc);

    origin_arg++;
  }

  // handle functbion arguments
//  auto func_arg = arguments.begin();
//  for (auto &arg_it: )

  block.GenerateIR(generator);

  return function;
}

llvm::Value *NIfStatement::GenerateIR(triode::ir::Generator &generator) {
  llvm::Value *cond = condition.GenerateIR(generator);

  cond = generator.GetBuilder().CreateIntCast(cond, llvm::Type::getInt1Ty(generator.GetContext()), true);

  llvm::Function* theFunction = generator.GetBuilder().GetInsertBlock()->getParent();

  llvm::BasicBlock *then = llvm::BasicBlock::Create(generator.GetContext(), "then", theFunction);
  llvm::BasicBlock *merge = llvm::BasicBlock::Create(generator.GetContext(), "ifcont");
  generator.GetBuilder().CreateCondBr(cond, then, merge);

  generator.GetBuilder().SetInsertPoint(then);
  trueBlock.GenerateIR(generator);

  generator.GetBuilder().CreateBr(merge);

  theFunction->getBasicBlockList().push_back(merge);
  generator.GetBuilder().SetInsertPoint(merge);

  return NULL;
}

llvm::Value *NBinaryOperator::GenerateIR(triode::ir::Generator &generator) {
  llvm::Value *l = lhs.GenerateIR(generator);
  llvm::Value *r = rhs.GenerateIR(generator);
  llvm::Value *out = generator.GetBuilder().CreateICmpEQ(l, r, "cmptmp");

  return out;
}

llvm::Value *NIdentifier::GenerateIR(triode::ir::Generator &generator) {
  return generator.GetBuilder().CreateLoad(generator.GetLocalValue(name), false, "");
}

llvm::Value *NVariableDeclaration::GenerateIR(triode::ir::Generator &generator) {
  llvm::Value *inst = generator.GetBuilder().CreateAlloca(generator.GetType("Packet"));

  return inst;
}

llvm::Value *NStructDeclaration::GenerateIR(triode::ir::Generator &generator) {
  auto packetStruct = llvm::StructType::create(generator.GetContext(), id.name);

  generator.AddStruct(id.name, packetStruct);

  std::vector<llvm::Type *> memberTypes;
  memberTypes.push_back(llvm::Type::getInt32Ty(generator.GetContext()));
  packetStruct->setBody(memberTypes);
  return NULL;
}

llvm::Value *NStructMember::GenerateIR(triode::ir::Generator &generator) {
  auto varPtr = generator.GetLocalValue(this->id.name);

//  auto structPtr = generator.GetBuilder().CreateLoad(varPtr, "structPtr");
//  structPtr->setAlignment(4); // need to adjust, this may be infect layout of packet struct

  std::string structName = id.name;
  long memberIndex = 0;

  std::vector<llvm::Value*> indices;
  indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(generator.GetContext()), 0, false));
  indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(generator.GetContext()), (uint64_t)memberIndex, false));
  auto ptr = generator.GetBuilder().CreateInBoundsGEP(varPtr, indices, "memberPtr");

  return generator.GetBuilder().CreateLoad(ptr);
}

llvm::Value *NMethodCall::GenerateIR(triode::ir::Generator &generator) {
  llvm::Function * calleeF = generator.GetModule()->getFunction("drop");
  std::vector<llvm::Value*> argsv;
  for(auto it=this->arguments.begin(); it!=this->arguments.end(); it++){
    argsv.push_back((*it)->GenerateIR(generator));
    if( !argsv.back() ){        // if any argument codegen fail
      return nullptr;
    }
  }

  return generator.GetBuilder().CreateCall(calleeF, argsv, "calltmp");
}

llvm::Value *NInteger::GenerateIR(triode::ir::Generator &generator) {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(generator.GetContext()), this->value, true);
}

}