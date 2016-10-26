#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept> // std::invalid_argument for g++ 4.8.2

#include "mcstats.h"
#include "parse.h"

using std::cout;
using std::endl;
//using namespace std;

doubles g_stack;

void print_stack()
{
	for( auto d: g_stack) { cout << d << endl; }
	//cout << endl;
}

void prstat(const char *name, double value) { cout << name << ": " << value << endl; }

void do_calcs()
{
	if(g_stack.size() == 0) { /* cout << "Stack empty" << endl; */ return; }
	
	stats s;
	basic_stats(g_stack, s);
	prstat("Size", s.n);
	prstat("Mean", s.mean);
	prstat("Stdev", s.stdev);

	sortd(g_stack);
	prstat("Median", quantile(g_stack, 0.5));
	g_stack.clear();
}

class Quit: public std::exception 
{
	public:
		//Quit(const std::string m="quit"):msg(m){};
		//~Quit();
		//const char* what(){ return msg.c_str();}
		virtual const char* what() const throw()  { return "quit"; }
	//private:
	//	std::string msg;
};


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
	//print_tokens(tokens);
	//std::cout << tokens << std::endl;
	
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
	std::string in;
        //while(std::getline(std::cin, in) ) {
	char *line;
	//initialise_readline();
	//bool done = false;
	//while(! done)  {
	while(true) {
		line = readline("");
		if(line == NULL)  throw Quit();
		//	done = true;
		//	continue;
		//}

		add_history(line);
		in = std::string(line);
		free(line);

		std::string no_commas = rmchar(in, ',');
		std::vector<std::string> tokens = tokenize_line(no_commas);
		for(auto t: tokens) process_token(t);
	}
}

main()
{
	try { repl(); }
	catch(Quit& e) {
		//cout << "caught quit" << endl;
		do_calcs();
	}
}
