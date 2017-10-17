#include <iostream>
#include "expr.h"
#include <ctype.h>
#include <cstdlib>
#include <string.h>
#include <stdio.h>

Expression * parse(const char ** strp);

void skipSpace(const char ** strp) {
  while(isspace(**strp)) {
    *strp = *strp + 1;
  }
}
Expression * makeExpr(char op, Expression * lhs, Expression * rhs) {
  switch(op) {
  case '+':
    return new PlusExpression(lhs,rhs);
  case '-':
  case '*':
  case '/':
    std::cerr << op << " will be implemented in the future!\n";
  default:
    std::cerr << "Impossible op char: " << op << "\n";
    abort();
  }
}
bool isValidOp(char c) {
  return strchr("+-*/", c) != NULL;
}


Expression * parseOp(const char ** strp) {
  skipSpace(strp);
  char op = **strp;
  if (!isValidOp(op)) {
    std::cerr << "Invalid op: "<< op<< "\n";
    return NULL;
  }
  *strp = *strp + 1;
  Expression * lhs = parse(strp);
  if (lhs == NULL) {
    return NULL;
  }
  Expression * rhs = parse(strp);
  if (rhs == NULL) {
    delete lhs;
    return NULL;
  }
  skipSpace(strp);
  if (**strp == ')') {
    *strp = *strp + 1;
    return makeExpr(op,lhs,rhs);
  }
  std::cerr <<"Expected ) but found " << *strp << "\n";
  delete lhs;
  delete rhs;
  return NULL;
}
Expression * parse(const char ** strp) {
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
    return NULL;
  }
  else if (**strp == '(') {
    // (op E E)
    *strp = *strp + 1;
    return parseOp(strp);
  }
  else {
    //number
    char * endp;
    long num = strtol(*strp, &endp, 10);
    if (endp == *strp) {
      std::cerr << "Expected a number, but found " << *strp << "\n";
      return NULL;
    }
    *strp = endp;
    return new NumExpression(num);
  }
}

int main(void) {
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, stdin) != -1) {
    const char * temp = line;
    std::cout << "Read expression: " << line;
    Expression * expr = parse(&temp);
    if (expr != NULL) {
      std::cout << "Parsed expression to: " << expr->toString() << "\n";
      //std::cout << "Evaluated expression to: " << expr->evaluate() << "\n";
      std::cout << "(evaluation will be done in the future)\n";
      delete expr;
    }
    else {
      std::cout << "Could not parse expression, please try again.\n";
    }
  }
  free(line);
  return EXIT_SUCCESS;
}
