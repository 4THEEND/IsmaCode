#pragma once

#include <iostream>
#include <string>
#include "Lexer.h"
#include "IsmObject.h"
#include "Expression.h"

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

class Parser
{
public:
	Parser(SourceCodeInstructions instructions, Tokens Tokens);
private:
	std::string TrimString(std::string&& chars);
	bool ExamVariable(const std::string& supposed_name);
	bool ParseFunctions(std::vector<Instruction> SourceCode, bool EntryPoint);
	bool ParseVariableName(const std::string& variable);
	bool ParseVariables(const Instruction& var_instruction);
	bool ParseFunctionCalls(const Instruction& var_instruction);
	bool ParseUnknow(const Instruction& var_instruction);
	bool ParseExpression(const std::string& expression);

	std::vector<IsmObject> m_Variables;

	Tokens m_Tokens;
};

