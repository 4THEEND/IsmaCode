#pragma once

#include <iostream>
#include <string>

#include "Lexer.h"
#include "IsmObject.h"
#include "IsmTree.h"

struct Tokens
{
	std::unordered_map<std::string, std::string>& Token;
	std::unordered_map<std::string, std::string>& TokenReversed;
};

struct StringInfos
{
	bool isVariable = true;
	std::string chain;
};

struct ReturnExpression
{
	bool isGood;
	Expression expression;
};

struct ReturnParsedFunctionCall
{
	bool isGood;
	std::vector<Expression> expression;
};

class Parser
{
public:
	Parser(SourceCodeInstructions instructions, Tokens Tokens);
	static std::string TrimString(std::string&& chars);
private:
	bool ExamVariable(const std::string& supposed_name);
	bool ParseFunctions(std::vector<Instruction> SourceCode, bool EntryPoint);
	bool ParseVariableName(const std::string& variable);
	bool ParseVariables(const Instruction& var_instruction);
	ReturnParsedFunctionCall ParseFunctionCalls(const Instruction& var_instruction);
	bool ParseLoops(const Instruction& var_instruction);
	bool ParseUnknow(const Instruction& var_instruction);
	ReturnExpression ParseExpression(const std::string& expression, unsigned int line = 0);

	std::vector<std::string> m_RelativeVariables;
	std::vector<IsmObject> m_Variables;
	std::shared_ptr<Node> m_lastNode;
	IsmTree m_SyntaxTree;
	Tokens m_Tokens;
};

