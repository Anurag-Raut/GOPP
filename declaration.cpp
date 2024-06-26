
#include "declarations/declaration.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "stmt.hpp"
#include "token.hpp"

using namespace std;

Statement::Statement(unique_ptr<Stmt> stmt) { this->stmt = std::move(stmt); }

void Statement::execute(Environment *env) { stmt->evaluate(env); }

VarDecl::VarDecl(string name, unique_ptr<Expr> value) {
  this->name = name;
  this->value = std::move(value);
}

void VarDecl::execute(Environment *env) {
  // cout<<"doing var declartion"<<endl;
  // Literal l1=value->evaluate(env);
  // l1.printLiteral();
  env->setVariable(name, value->evaluate(env));
  // variables[name].printLiteral();
}

BlockDecl::BlockDecl(unique_ptr<vector<unique_ptr<Decl>>> decls) {
  this->decls = std::move(decls);
}

void BlockDecl::execute(Environment *env) {
  Environment *childEnv = new Environment();
  childEnv->parent = env;
  for (const auto &decl : *decls) {
    decl->execute(childEnv);
  }
}

IfDecl::IfDecl(unique_ptr<Expr> expr, unique_ptr<BlockDecl> ifBlock,
               unique_ptr<BlockDecl> elseBlock) {
  this->expr = std::move(expr);
  this->ifBlock = std::move(ifBlock);
  this->elseBlock = std::move(elseBlock);
}
IfDecl::IfDecl(unique_ptr<Expr> expr, unique_ptr<BlockDecl> ifBlock) {
  this->expr = std::move(expr);
  this->ifBlock = std::move(ifBlock);
}

void IfDecl::execute(Environment *env) {
  Literal ans = expr->evaluate(env);
  if (!ans.checkType<bool>()) {
    ifBlock->execute(env);

  } else if (ans.checkType<bool>() && any_cast<bool>(ans.value)) {
    ifBlock->execute(env);

  } else {
    elseBlock->execute(env);
  }
}

WhileBlock::WhileBlock(unique_ptr<Expr> expr, unique_ptr<BlockDecl> block) {
  this->expr = std::move(expr);
  this->block = std::move(block);
}

void WhileBlock::execute(Environment *env) {
  Literal ans = expr->evaluate(env);
  while (!ans.checkType<bool>() ||
         ans.checkType<bool>() && any_cast<bool>(ans.value)) {
    block->execute(env);
    ans = expr->evaluate(env);
  }
}

FuncDecl::FuncDecl(string name,
                   unique_ptr<vector<unique_ptr<Parameter>>> parameters,
                   unique_ptr<BlockDecl> block) {
  this->name = name;
  this->parameters = std::move(parameters);
  this->block = std::move(block);
}
void FuncDecl::execute(Environment *env) { block->execute(env); }

// getVariable(){

// }
