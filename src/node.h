//
// Created by gd on 2018/10/15.
//

#ifndef TRIODE_NODE_H
#define TRIODE_NODE_H

#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>
//#include "ir/generator.h"

namespace triode {

namespace ir {
class Generator;
}

class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement *> StatementList;
typedef std::vector<NExpression *> ExpressionList;
typedef std::vector<NVariableDeclaration *> VariableList;

class Node {
 public:
  virtual ~Node() {}

  virtual llvm::Value *GenerateIR(triode::ir::Generator &) { return NULL; }
};

class NExpression : public Node {

};

class NStatement : public Node {
};

class NBlock : public Node {
 public:
  StatementList statements;
  NBlock() {}

  llvm::Value *GenerateIR(triode::ir::Generator &);

};

class NInteger : public NExpression {
 public:
  long long value;
  NInteger(long long value) : value(value) {}
  llvm::Value *GenerateIR(triode::ir::Generator &generator);
};

class NIdentifier : public NExpression {
 public:
  std::string name;
  NIdentifier(const std::string &name) : name(name) {}
  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NAssignment : public NExpression {
 public:
  NIdentifier &lhs;
  NExpression &rhs;
  NAssignment(NIdentifier &lhs, NExpression &rhs) :
      lhs(lhs), rhs(rhs) {};

//  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NVariableDeclaration : public NStatement {
 public:
  const NIdentifier &type;
  NIdentifier &id;
  NExpression *assignmentExpr;
  NVariableDeclaration(const NIdentifier &type, NIdentifier &id) :
      type(type), id(id) { assignmentExpr = NULL; }
  NVariableDeclaration(const NIdentifier &type, NIdentifier &id, NExpression *assignmentExpr) :
      type(type), id(id), assignmentExpr(assignmentExpr) {};

  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NFunctionDeclaration : public NStatement {
 public:
  const NIdentifier &type;
  const NIdentifier &id;
  VariableList arguments;
  NBlock &block;

  NFunctionDeclaration(const NIdentifier &type, const NIdentifier &id,
                       const VariableList &arguments, NBlock &block) :
      type(type), id(id), arguments(arguments), block(block) {};

  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NIfStatement : public NStatement {
 public:
  NExpression &condition;
  NBlock &trueBlock;
  NBlock &falseBlock;
  int hasElse;

  NIfStatement(NExpression &condition, NBlock &trueBlock)
      : condition(condition), trueBlock(trueBlock), falseBlock(trueBlock) { hasElse = 0; }

  NIfStatement(NExpression &condition, NBlock &trueBlock, NBlock &falseBlock)
      : condition(condition), trueBlock(trueBlock), falseBlock(falseBlock) { hasElse = 1; };

  llvm::Value *GenerateIR(triode::ir::Generator &);

};

class NBinaryOperator : public NExpression {
 public:
  int op;
  NExpression &lhs;
  NExpression &rhs;
  NBinaryOperator(NExpression &lhs, int op, NExpression &rhs)
      : lhs(lhs), op(op), rhs(rhs) {};

  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NReturnStatement : public NStatement {
 public:
  NExpression &expression;
  NReturnStatement(NExpression &expression) :
      expression(expression) {};
//  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NMethodCall : public NExpression {
 public:
  const NIdentifier &id;
  ExpressionList arguments;
  NMethodCall(const NIdentifier &id, ExpressionList &arguments) :
      id(id), arguments(arguments) {}
  NMethodCall(const NIdentifier &id) : id(id) {};
  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NStructDeclaration : public NStatement {
 public:
  const NIdentifier &id;
  VariableList members;

  NStructDeclaration(const NIdentifier &id, const VariableList &members)
      : id(id), members(members) {};

  llvm::Value *GenerateIR(triode::ir::Generator &);
};

class NStructMember : public NExpression {
 public:
  const NIdentifier &id;
  const NIdentifier &member;

  NStructMember(const NIdentifier &id, const NIdentifier &member) : id(id), member(member) {};

  llvm::Value *GenerateIR(triode::ir::Generator &);
};

}

#endif //TRIODE_NODE_H
