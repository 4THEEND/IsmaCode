#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#define TOKEN_FILE "Tokens"
#define RESERVED 30

struct Instruction
{
	std::string identificator;
	std::string line;
	unsigned int n_line;
};

using SourceCodeInstructions = std::unordered_map<std::string, std::vector<Instruction>>;

class Lexer
{
public:
	Lexer(const std::string& file);
	bool isGood;
	inline SourceCodeInstructions getInstructions() { return m_FileLexed; }
	inline std::unordered_map<std::string, std::string>& getToken() { return m_Tokens; }
	inline std::unordered_map<std::string, std::string>& getTokenReversed() { return m_TokensReversed; }
private:	
	friend extern class Parser;
	SourceCodeInstructions m_FileLexed;
	static bool isFunctionCall(const std::string& line);
	static std::unordered_map<std::string, std::string> m_Tokens;
	static std::unordered_map<std::string, std::string> m_TokensReversed;
	static void getTokens();
};

/*
this class is used to transform a file in a list of tokens // add namespaces 

next: make a class to transform this list of tokens in a tree of instructions // search for variables functions & referneces // hash variable names & functions
manage parenthesis ()
next1: make error class
next2: runtime execution
*/

