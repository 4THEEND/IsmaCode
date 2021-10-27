#pragma once

#include <iostream>
#include <string>
#include "Lexer.h"
#include "IsmObject.h"

struct Tokens
{
	std::unordered_map<std::string, std::string>& Token;
	std::unordered_map<std::string, std::string>& TokenReversed;
};

class Parser
{
public:
	Parser(SourceCodeInstructions instructions, Tokens Tokens);
private:
	bool ParseFunctions(std::vector<Instruction> SourceCode, bool EntryPoint);
	std::string TrimString(std::string&& chars);
	bool ParseVariableName(const std::string& variable);
	void ParseVariables();

	Tokens m_Tokens;
};

