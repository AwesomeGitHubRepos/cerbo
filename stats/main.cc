#include <iostream>
#include <stdio.h>
#include <string>
using std::string;


class Quit: public std::exception 
{
	public:
		virtual const char* what() const throw()  { return "quit"; }
};

#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>
#include <readline/history.h>
#define READLINE_TEXT "readline: yes"
string rdline()
{
	string result;
	char *line = readline("");
	if(line == NULL) throw Quit();
	add_history(line);
	result = string(line);
	free(line);
	return result;
}

#else
#define READLINE_TEXT "readline: no"
string rdline()
{
	string result;
	std::istream& ret = std::getline(std::cin, result);
	//if(result.size() == 0 && ret == std::eofbit) throw Quit();
	if(result.size() == 0 && std::cin.eof()) throw Quit();
	return result;
}
#endif

#include <stdlib.h>
#include <exception>
#include <stdexcept> // std::invalid_argument for g++ 4.8.2

#include "stats.hpp"
#include "parse.hpp"

using std::cout;
using std::endl;
//using namespace std;

doubles g_stack;

void print_stack()
{
	for( auto d: g_stack) { cout << d << endl; }
}

void prstat(const char *name, double value)
{ 
	cout << name << ": " << value << endl; 
}

void do_calcs()
{
	if(g_stack.size() == 0) { /* cout << "Stack empty" << endl; */ return; }
	
	const stats_t s = basic_stats(g_stack);
	prstat("Size", s.n);
	prstat("Mean", s.mean);
	prstat("Sum", s.sum);
	prstat("Stdev", s.stdev);

	sortd(g_stack);
	prstat("Median", quantile(g_stack, 0.5));
	g_stack.clear();
}



void print_tokens(std::vector<std::string> tokens)
{
	for(auto t: tokens) { cout << "Token: *" << t << "*" << endl; }
	cout << endl;
}

std::string rmchar(std::string str, char c)
{
	std::string res;
	for(auto s: str) {if (s != c) res += s;}
	return res;
}

void process_token(std::string token)
{
	
	if(token== ".s") { print_stack() ; return; }

	if(token == "q") { throw Quit(); }

	if(token == "g") { do_calcs() ; return; }

	// should be a number that we put onto the stack
	try { 
		double d = stod(token);
		g_stack.push_back(d);
	}
	catch (std::invalid_argument& e) { cout << "Skipping token: " << token << endl; }
	catch (std::out_of_range& e) { cout << "Out of range: " << token << endl; }
	
}

void repl()
{
	//std::string in;
	//char *line;
	while(true) {
		string line = rdline();
		//line = readline("");
		//if(line == NULL)  throw Quit();
		//add_history(line);
		//in = std::string(line);
		//free(line);

		std::string no_commas = rmchar(line, ',');
		std::vector<std::string> tokens = tokenize_line(no_commas);
		for(auto t: tokens) process_token(t);
	}
}

main()
{
	cout << READLINE_TEXT << endl;

	try { repl(); }
	catch(Quit& e) {
		do_calcs();
	}
}
