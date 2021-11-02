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
	m_Variables = {};
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
				{
					args.emplace_back(variable);
					m_Variables = args;
				}
				else
				{
					Log::Error("Bad argument variable name at line " + std::to_string(SourceCode[0].n_line));
					return false;
				}
			}
		}
	}

	/*parse variables
	functions storage
	parse conditions
	parse loops
	expressions in parenthesis
	Parse code and variables*/
	
	m_lastNode = m_SyntaxTree.clear();
	for (unsigned int i = 0; i < SourceCode.size(); i++)
	{
		if (SourceCode[i].identificator == VAR_DECLARATOR)
		{
			if (!ParseVariables(SourceCode[i]))
				return false;
		}
		else if(SourceCode[i].identificator == UNKNOWN_INSTRUCTION)
		{
			if (!ParseUnknow(SourceCode[i]))
				return false;
		}
		else if (SourceCode[i].identificator == WHILE_DECLARATOR || SourceCode[i].identificator == FOR_DECLARATOR)
		{
			if (!ParseLoops(SourceCode[i]))
				return false;
		}
		else if (SourceCode[i].identificator == FUNCTION_CALL_STATEMENT)
		{
			if (!ParseFunctionCalls(SourceCode[i]).isGood)
				return false;
		}
	}
	return true;
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

bool Parser::ExamVariable(const std::string& supposed_name)
{
	for (const IsmObject& variable : m_Variables)
	{
		if (variable.m_name == supposed_name)
			return true;
	}
	for (const auto& variable : m_RelativeVariables)
	{
		if (variable == supposed_name)
			return true;
	}
	return false;
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

bool Parser::ParseVariables(const Instruction& var_instruction)
{
	size_t pos_assignement = var_instruction.line.find(m_Tokens.TokenReversed[EQ_STATEMENT]);
	if (pos_assignement == std::string::npos)
	{
		Log::Error("Variables must be initialised at line " + std::to_string(var_instruction.n_line));
		return false;
	}
	size_t sizeVarDecl = m_Tokens.TokenReversed[VAR_DECLARATOR].size();
	std::string var_name = TrimString(var_instruction.line.substr(sizeVarDecl + 1, pos_assignement - sizeVarDecl - 1));
	if (var_name.empty())
	{
		Log::Error("Variable names must respect Camel case convention at line " + std::to_string(var_instruction.n_line));
		return false;
	}
	std::string variable_content = TrimString(var_instruction.line.substr(pos_assignement + 1));
	ReturnExpression parsed = ParseExpression(variable_content);
	if (!parsed.isGood)
	{
		Log::Error("Bad variable content at line " + std::to_string(var_instruction.n_line));
		return false;
	}
	else
	{
		AppendResult result =  m_SyntaxTree.append({ VAR_DECLARATOR, parsed.expression }, m_lastNode);
		if (!result.isGood)
		{
			Log::Error("Error in tree appending");
			return false;
		}
		else
			m_lastNode = result.lastNode;
	}
	m_Variables.emplace_back(var_name);
	return true;
}

ReturnParsedFunctionCall Parser::ParseFunctionCalls(const Instruction& var_instruction)
{
	unsigned int Dargspos = var_instruction.line.find(m_Tokens.TokenReversed[L_PAR_STATEMENT]);
	unsigned int Fargspos = var_instruction.line.rfind(m_Tokens.TokenReversed[R_PAR_STATEMENT]);

	std::string Arguments = TrimString(var_instruction.line.substr(Dargspos + 1, Fargspos - Dargspos - 1));
	unsigned int ParCompt = 0;
	std::vector<std::string> ArgumentsDecompose{};
	std::string argument{};
	for (const char character : Arguments)
	{
		std::string character2{ std::string(1, character) };
		if (character2 == m_Tokens.TokenReversed[SEPARATOR_STATEMENT] && ParCompt == 0)
		{
			ArgumentsDecompose.push_back(TrimString(std::move(argument)));
			argument.clear();
			continue;
		}
		else if (character2 == m_Tokens.TokenReversed[L_PAR_STATEMENT])
		{
			ParCompt++;
		}
		else if (character2 == m_Tokens.TokenReversed[R_PAR_STATEMENT])
		{
			ParCompt--;
		}
		argument += character;
	}
	if (!argument.empty())
		ArgumentsDecompose.push_back(TrimString(std::move(argument)));

	std::vector<Expression> parsedArgsExpressions;
	for (std::string& argument_ : ArgumentsDecompose)
	{
		ReturnExpression parsed = ParseExpression(argument_, var_instruction.n_line);
		if (!parsed.isGood)
		{
			Log::Error("Error while parsing arguments at line " + std::to_string(var_instruction.n_line));
			return { false, {} };
		}
		parsedArgsExpressions.push_back(parsed.expression);
	}
	//TODO parse this
	return { true, parsedArgsExpressions };
}

bool Parser::ParseLoops(const Instruction& var_instruction)
{
	//Oversimplifing
	m_RelativeVariables.push_back("i");
	std::cout << "Loop:  " << var_instruction.line << " " <<  var_instruction.identificator << std::endl;
	//TODO: parse this
	return true;
}

bool Parser::ParseUnknow(const Instruction& var_instruction)
{
	size_t pos_assignement = var_instruction.line.find(m_Tokens.TokenReversed[EQ_STATEMENT]);
	if (pos_assignement == std::string::npos)
	{
		Log::Error("Unknow instruction at line " + std::to_string(var_instruction.n_line));
		return false;
	}

	std::string supposed_name = TrimString(var_instruction.line.substr(0, pos_assignement));
	if (!ParseVariableName(supposed_name))
	{
		Log::Error("Bad variable name at line " + std::to_string(var_instruction.n_line));
		return false;
	}
	
	if (!ExamVariable(supposed_name))
	{
		Log::Error("Unwnown variable at line " + std::to_string(var_instruction.n_line));
		return false;
	}
	std::string variable_content = TrimString(var_instruction.line.substr(pos_assignement + 1));
	ReturnExpression parsed = ParseExpression(variable_content);
	if (!parsed.isGood)
	{
		Log::Error("Bad variable assignation content at line " + std::to_string(var_instruction.n_line));
		return false;
	}
	else
	{
		AppendResult result = m_SyntaxTree.append({ VAR_ASSIGN, parsed.expression }, m_lastNode);
		if (!result.isGood)
		{
			Log::Error("Error in tree appending");
			return false;
		}
		else
			m_lastNode = result.lastNode;
	}
	return true;
}

ReturnExpression Parser::ParseExpression(const std::string& expression, unsigned int line)
{
	bool isNumber = false;
	bool isString = false;
	bool FunctionCall = false;
	std::string functionCall;
	int ParComptRemember = 0;
	bool ParCompReSetted = false;
	StringInfos variables;
	int ParCompt = 0;
	std::string numAdd{};

	std::vector<std::vector<ExpressionDecomp>> infos(
		expression.size(),
		std::vector<ExpressionDecomp>(0)
	);

	for (const char character : expression)
	{
		auto character2 = std::string(1, character);
		if (FunctionCall)
		{
			functionCall += character;
		}
		if (character2 == m_Tokens.TokenReversed[QUO2_STATEMENT] || character2 == m_Tokens.TokenReversed[QUO_STATEMENT])
		{
			//TODO difference between str with quo and quo2
			if (!isString)
			{
				isString = true;
				variables.isVariable = false;
			}
			else
			{
				isString = false;
				infos[ParCompt].emplace_back(LITTERAL, variables.chain);
				variables = {};
			}
		}
		else if (isalpha(character) && !FunctionCall)
		{
			if (isNumber)
			{
				numAdd.find(m_Tokens.TokenReversed[POINT]) != std::string::npos ? infos[ParCompt].emplace_back(FLOAT, numAdd) : infos[ParCompt].emplace_back(INTEGER, numAdd);
				infos[ParCompt].emplace_back(MUL_OP, m_Tokens.TokenReversed[MUL_OP]);
				numAdd.clear();
				isNumber = false;
			}
			variables.chain += character;
		}
		else if (isString && !variables.isVariable)
		{
			variables.chain += character;
		}
		else
		{
			if (isdigit(character) && !FunctionCall)
			{
				numAdd += character;
				if (!isNumber)
					isNumber = true;
			}
			else if (isNumber)
			{
				numAdd.find(m_Tokens.TokenReversed[POINT]) != std::string::npos ? infos[ParCompt].emplace_back(FLOAT, numAdd) : infos[ParCompt].emplace_back(INTEGER, numAdd);
				numAdd.clear();
				isNumber = false;
			}

			bool lastPar = false;
			if (character2 == m_Tokens.TokenReversed[POINT])
			{
				if (isNumber)
					numAdd += character2;
				//TODO: members functions
			}
			else if (character2 == m_Tokens.TokenReversed[L_PAR_STATEMENT])
			{
				ParCompt++;
				if (variables.chain.empty())
				{
					continue;
				}
				else
				{
					lastPar = true;
				}
			}
			else if (character2 == m_Tokens.TokenReversed[R_PAR_STATEMENT])
			{
				ParCompt--;
				if (FunctionCall && ParCompt == ParComptRemember)
				{
					auto result = ParseFunctionCalls({ FUNCTION_CALL_STATEMENT , functionCall, line });
					if (!result.isGood)
						return { false, {{}} };
					infos[ParCompt].emplace_back(FUNCTION_CALL_STATEMENT, functionCall, result.expression);
					FunctionCall = false;
					ParCompReSetted = false;
				}
				else
				{
					continue;
				}
			}
			if (!variables.chain.empty())
			{
				if (lastPar)
				{
					FunctionCall = true;
					if (!ParCompReSetted)
					{
						ParComptRemember = ParCompt - 1;
						ParCompReSetted = true;
					}
					functionCall = variables.chain + m_Tokens.TokenReversed[L_PAR_STATEMENT];
				}
				else
				{
					if (ExamVariable(variables.chain))
					{
						infos[ParCompt].emplace_back(VAR_DECLARATOR, variables.chain);
					}
					else
						return { false, {{}} };
				}
				variables = {};
			}
			else if (character2 == m_Tokens.TokenReversed[PLUS_OP] || character2 == m_Tokens.TokenReversed[MINUS_OP] ||
					character2 == m_Tokens.TokenReversed[MUL_OP] || character2 == m_Tokens.TokenReversed[DIV_OP])
			{
				infos[ParCompt].emplace_back(m_Tokens.Token[character2], character2);
			}
		}
	}

	if (ParCompt != 0)
	{
		Log::Error("Error in parenthesis count");
		return { false, {{}} };
	}
	else if (isNumber)
		numAdd.find(m_Tokens.TokenReversed[POINT]) != std::string::npos ? infos[ParCompt].emplace_back(FLOAT, numAdd) : infos[ParCompt].emplace_back(INTEGER, numAdd);
	else if (!variables.chain.empty())
	{
		if (ExamVariable(variables.chain))
			infos[ParCompt].emplace_back(VAR_DECLARATOR, variables.chain);
		else
			return { false, {{}} };
	}
	return { true, infos };
}
