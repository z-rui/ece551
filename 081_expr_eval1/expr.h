#include <string>
#include <sstream>

class Expression {
public:
	virtual std::string toString() const = 0;
	virtual ~Expression() {}
};

class NumExpression : public Expression {
	long numVal;
public:
	NumExpression(long v) : numVal(v) {}
	virtual std::string toString() const {
		std::stringstream ss;
		ss << numVal;
		return ss.str();
	}
};

class BinOpExpression : public Expression {
	const char *theOp; // always points to a literal
	Expression *lhs, *rhs; // owned by the object
public:
	BinOpExpression(const char *op, Expression *lhs, Expression *rhs) : theOp(op), lhs(lhs), rhs(rhs) {}
	virtual std::string toString() const {
		std::stringstream ss;
		ss << '(' << lhs->toString() << ' ' << theOp << ' ' << rhs->toString() << ')';
		return ss.str();
	}
	virtual ~BinOpExpression() {
		delete lhs;
		delete rhs;
	}
};

class PlusExpression : public BinOpExpression {
public:
	PlusExpression(Expression *lhs, Expression *rhs) : BinOpExpression("+", lhs, rhs) {}
};
