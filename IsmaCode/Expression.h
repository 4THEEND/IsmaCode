#pragma once

#include <iostream>
#include <vector>

struct ExpressionDecomp
{
	std::string type;
	std::string instruction;
};

class Expression
{
public:
	Expression(std::vector<std::vector<ExpressionDecomp>> expression);
private:
	std::vector<std::vector<ExpressionDecomp>> m_Expression;
};

