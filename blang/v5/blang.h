#pragma once
#include <memory>
#include <string>
#include <variant>
#include <vector>
using namespace std::literals;

inline std::vector<std::string> strvec;
// 0 => end of file
//enum { LET = 1, IDENTIFIER }; 

//enum { CHAR = 1, IDENTIFIER, LET, NUMBER };

extern int yylex();
//typedef std::variant<std::string, double, int> token_t;
//#define YYSTYPE token_t
/*
struct YYSTYPE {
	int an_int;
	//std::shared_ptr<std::string> str;
	std::string str;
};

inline struct YYSTYPE yylval1;
*/
