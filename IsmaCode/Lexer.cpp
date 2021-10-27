#include "Lexer.h"
#include "Logging.h"

#include <fstream>

Lexer::Lexer(const std::string& file)
	:isGood(false)
{
	if (m_Tokens.empty())
		this->getTokens();

	std::ifstream SourceFile(file);
	if (SourceFile.fail())
	{
		Log::Error("Error while opening the source file");
		isGood = false;
		return;
	}
	else
	{
		std::string SourceLine{};
		std::string ScopeName = "main";
		std::vector<Instruction> ScopeInstruction;
		ScopeInstruction.reserve(RESERVED);
		int OpenAccCount = 0;
		unsigned int LineCount = 0;
		while (std::getline(SourceFile, SourceLine))
		{
			LineCount++;
			if (SourceLine.empty())
				continue;

			//Delete leading spaces
			SourceLine = SourceLine.substr(SourceLine.find_first_not_of(" \n\r\t\f\v"));
			std::string Declarator = SourceLine.substr(0, SourceLine.find(" "));
			if (m_Tokens[Declarator] == COMMENT_STATEMENT)
				continue;
			
			//Register a namspace
			if (m_Tokens[Declarator] == FUNCTION_DECLARATOR)
			{
				if (ScopeName == m_TokensReversed[MAIN_SCOPE_DECLARATOR])
				{
					if (!m_FileLexed[ScopeName].empty())
						m_FileLexed[ScopeName].insert(m_FileLexed[ScopeName].begin(), ScopeInstruction.begin(), ScopeInstruction.end());
					else
						m_FileLexed[ScopeName] = ScopeInstruction;
					ScopeInstruction = {};
				}
				int EndFunc = SourceLine.find(m_TokensReversed[L_PAR_STATEMENT]);
				int StartFunc = Declarator.length();
				std::string FunctionName = SourceLine.substr(StartFunc + 1, EndFunc - StartFunc - 1);
				ScopeName = FunctionName;
				ScopeInstruction.emplace_back("FUNCTION:" + FunctionName, SourceLine, LineCount);
			}
			else if (m_Tokens[Declarator] == L_ACC_DECLARATOR)
			{
				ScopeInstruction.emplace_back(m_Tokens[Declarator], SourceLine, LineCount);
				++OpenAccCount;
			}
			else if (m_Tokens[Declarator] == R_ACC_DECLARATOR)
			{
				ScopeInstruction.emplace_back(m_Tokens[Declarator], SourceLine, LineCount);
				--OpenAccCount;
				if (OpenAccCount == 0)
				{
					if (!m_FileLexed[ScopeName].empty())
						Log::Error("Function or Scope name already exist");
					else
					{
						m_FileLexed[ScopeName] = ScopeInstruction;
						ScopeInstruction = {};
					}
					ScopeName = m_TokensReversed[MAIN_SCOPE_DECLARATOR];
				}
				else if (OpenAccCount < 0)
					Log::Warning("Too many closed parenthesis");
			}
			else if (m_Tokens[Declarator] == VAR_DECLARATOR)
				ScopeInstruction.emplace_back(VAR_DECLARATOR, SourceLine, LineCount);
			else if (m_Tokens[Declarator] == IF_DECLARATOR || m_Tokens[Declarator] == ELSE_DECLARATOR
				|| m_Tokens[Declarator] == ELIF_DECLARATOR || m_Tokens[Declarator] == FOR_DECLARATOR
				|| m_Tokens[Declarator] == WHILE_DECLARATOR || m_Tokens[Declarator] == RETURN_DECLARATOR)
				ScopeInstruction.emplace_back(m_Tokens[Declarator], SourceLine, LineCount);
			else
			{
				if (isFunctionCall(SourceLine))
					ScopeInstruction.emplace_back(FUNCTION_CALL_STATEMENT, SourceLine, LineCount);
				else
					ScopeInstruction.emplace_back(UNKNOWN_INSTRUCTION, SourceLine, LineCount);
			}
		}
		if (!m_FileLexed[ScopeName].empty())
			m_FileLexed[ScopeName].insert(m_FileLexed[ScopeName].begin(), ScopeInstruction.begin(), ScopeInstruction.end());
		else
			m_FileLexed[ScopeName] = ScopeInstruction;
	}
	isGood = true;
}

void Lexer::getTokens()
{
	std::ifstream tokens_file;
	tokens_file.open(TOKEN_FILE, std::ios::in);
	if (tokens_file.fail())
	{
		Log::Error("Error while opening the token file");
	}
	else
	{
		std::string line{};
		while (std::getline(tokens_file, line))
		{
			int separation = line.find(" ");
			std::string identificateur = line.substr(0, separation);
			if (identificateur.empty())
				continue;
			line = line.substr(separation + 1, line.length() - 1);
			m_Tokens[line] = identificateur;
			m_TokensReversed[identificateur] = line;
		}
	}
	tokens_file.close();
}

bool Lexer::isFunctionCall(const std::string& line)
{
	int posParent = line.find(m_TokensReversed[L_PAR_STATEMENT]);
	int seconPos = line.rfind(m_TokensReversed[R_PAR_STATEMENT]);
	std::string line2 = line.substr(0, posParent);
	const char* line_remasterised = line2.c_str();
	if (posParent != line.npos && posParent < seconPos)
	{
		for (int i = 0; i < posParent; i++)
		{
			if (!isalpha(line_remasterised[i]))
				return false;
		}
		return true;
	}
	return false;
}

std::unordered_map<std::string, std::string> Lexer::m_Tokens;
std::unordered_map<std::string, std::string> Lexer::m_TokensReversed;