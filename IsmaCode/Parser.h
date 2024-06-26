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

enum class ParseType {
	Normal,
	Condition
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
	bool ParseLoops(Instruction& var_instruction, const int accCount);
	bool ParseUnknow(const Instruction& var_instruction);
	bool ParseReturn(const Instruction& var_instruction);
	ReturnExpression ParseExpression(const std::string& expression, unsigned int line = 0, ParseType type = ParseType::Normal);

	std::vector<std::vector<IsmObject>> m_RelativeVariables;
	std::vector<IsmObject> m_Variables;
	std::shared_ptr<Node> m_lastNode;
	IsmTree m_SyntaxTree;
	Tokens m_Tokens;
};

