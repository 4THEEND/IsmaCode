#include "Parser.h"
#include "Logging.h"

Parser::Parser(SourceCodeInstructions instructions, Tokens Tokens)
	:m_Tokens(Tokens)
{
	for (const auto& Environnement : instructions)
	{
		bool entry;
		Environnement.first == m_Tokens.TokenReversed[MAIN_SCOPE_DECLARATOR] ? entry = true : entry = false;
		if (!ParseFunctions(Environnement.second, entry))
		{
			Log::Error("Parsing aborted");
			return;
		}
	}
	Log::Info("Good Parsing");
}

bool Parser::ParseFunctions(std::vector<Instruction> SourceCode, bool EntryPoint)
{
	int IndStart = 0;
	EntryPoint ? IndStart = 0 : IndStart = 1;

	//Parse argument variables
	if (!EntryPoint) {
		std::vector<size_t> ArgsPos{};
		unsigned int count = 0;
		size_t pos = SourceCode[0].line.find(m_Tokens.TokenReversed[SEPARATOR_STATEMENT]);

		unsigned int Dargspos = SourceCode[0].line.find(m_Tokens.TokenReversed[L_PAR_STATEMENT]);
		unsigned int Fargspos = SourceCode[0].line.rfind(m_Tokens.TokenReversed[R_PAR_STATEMENT]);

		if (pos != std::string::npos)
		{
			while (pos != std::string::npos)
			{
				ArgsPos.push_back(pos);
				if (Dargspos > pos || Fargspos < pos)
				{
					Log::Error("Error in function declaration at line " + std::to_string(SourceCode[0].n_line));
					return false;
				}

				count++;
				pos = SourceCode[0].line.find(m_Tokens.TokenReversed[SEPARATOR_STATEMENT], pos + count);
			}
		}
		if (!ArgsPos.empty() || Dargspos != Fargspos - 1)
		{
			if (std::count(SourceCode[0].line.begin(), SourceCode[0].line.end(), (char)m_Tokens.TokenReversed[L_PAR_STATEMENT].c_str()[0]) != 1 ||
				std::count(SourceCode[0].line.begin(), SourceCode[0].line.end(), (char)m_Tokens.TokenReversed[R_PAR_STATEMENT].c_str()[0]) != 1)
			{
				Log::Error("Syntax error at line " + std::to_string(SourceCode[0].n_line));
				return false;
			}
			std::vector<std::string> args_tempo;
			args_tempo.reserve(ArgsPos.size() + 1);
			if (ArgsPos.empty())
			{
				args_tempo.emplace_back(TrimString(SourceCode[0].line.substr(Dargspos + 1, Fargspos - Dargspos - 1)));
			}
			else
			{
				int lastPos = Dargspos;
				for (size_t i : ArgsPos)
				{
					args_tempo.emplace_back(TrimString(SourceCode[0].line.substr(lastPos + 1, i - lastPos - 1)));
					lastPos = i;
				}
				args_tempo.emplace_back(TrimString(SourceCode[0].line.substr(lastPos + 1, Fargspos - lastPos - 1)));
			}
			std::vector<IsmObject> args;
			args.reserve(args_tempo.size());
			for (const std::string& variable : args_tempo)
			{
				if (ParseVariableName(variable))
					args.emplace_back(variable);
				else
				{
					Log::Error("Bad argument variable name at line " + std::to_string(SourceCode[0].n_line));
					return false;
				}
			}
		}
	}
	for (int i = 0; i < SourceCode.size(); i++)
		std::cout << SourceCode[i].line << std::endl;

	return true;
	//parse variables
	//expressions in parenthesis
	//Parse code and variables
}

std::string Parser::TrimString(std::string&& chars)
{
	const std::string spaces = " \n\r\t\f\v";
	size_t Dwhitespaces = chars.find_first_not_of(spaces);
	size_t Ewhitespaces = chars.find_last_not_of(spaces);
	if (Dwhitespaces == std::string::npos || Ewhitespaces == std::string::npos)
		return "";
	else
		return chars.substr(Dwhitespaces, Ewhitespaces - Dwhitespaces + 1);
}

bool Parser::ParseVariableName(const std::string& variable)
{
	const char* line_remasterised = variable.c_str();
	for (size_t i = 0; i < variable.length(); i++)
	{
		if (!isalpha(line_remasterised[i]))
			return false;
	}
	return true;
}

void Parser::ParseVariables()
{
}
