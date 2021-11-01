#pragma once

#include <iostream>
#include <vector>

struct ExpressionDecomp;

class Expression
{
public:
	Expression(std::vector<std::vector<ExpressionDecomp>> expression);
private:
	friend extern class Parser;
	std::vector<std::vector<ExpressionDecomp>> m_Expression;
};

struct ExpressionDecomp
{
	std::string type;
	std::string instruction;
	std::vector<Expression> NestedExpressions = {};
};
