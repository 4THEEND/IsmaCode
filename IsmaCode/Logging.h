#pragma once

#include <iostream>
#include <string>

#define ENTRY_POINT "EP"
#define MAIN_SCOPE_DECLARATOR "MAIN_SCOPE"
#define IMPORT_STATEMENT "IMPORT"
#define VAR_DECLARATOR "VAR"
#define VAR_ASSIGN "VAR_ASS"
#define FUNCTION_DECLARATOR "FUNCTION"
#define RETURN_DECLARATOR "RETURN"
#define IF_DECLARATOR "IF"
#define ELSE_DECLARATOR "ELSE"
#define ELIF_DECLARATOR "ELIF"
#define FOR_DECLARATOR "FOR"
#define WHILE_DECLARATOR "WHILE"
#define COND_EQ_STATEMENT "COND_EQ"
#define TO_STATEMENT "TO"
#define IN_STATEMENT "IN"
#define COND_INF_EQ_STATEMENT "COND_INF_EQ"
#define COND_SUP_EQ_STATEMENT "COND_SUP_EQ"
#define COND_DIFF_STATEMENT "COND_DIFF"
#define EQ_STATEMENT "EQ"
#define QUO_STATEMENT "QUO"
#define QUO2_STATEMENT "QUO2"
#define L_ACC_DECLARATOR "L_ACC"
#define R_ACC_DECLARATOR "R_ACC"
#define L_PAR_STATEMENT "L_PAR"
#define R_PAR_STATEMENT "R_PAR"
#define COMMENT_STATEMENT "COMMENT"
#define BOOl_TRUE_KEYWORD "BOOl_TRUE"
#define BOOl_FALSE_KEYWORD "BOOl_FALSE"
#define FUNCTION_CALL_STATEMENT "FUNCTION_CALL"
#define UNKNOWN_INSTRUCTION "UNKNOWN_INSTRUCTION"
#define SEPARATOR_STATEMENT "SEPARATOR"
#define POINT "POINT"

#define LITTERAL "LITT"
#define INTEGER "INT"
#define FLOAT "FLOAT"

#define PLUS_OP "PLUS"
#define MINUS_OP "MINUS"
#define MUL_OP "MUL"
#define DIV_OP "DIV"


namespace Log
{
	//Classe d'errreur et affichage messages d'erreurs 
	void Info(const std::string info);
	void Warning(const std::string warning);
	void Error(const std::string error);
};
