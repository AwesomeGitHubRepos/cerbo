// http://theboostcpplibraries.com/boost.tokenizer
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>

#include <fstream>
#include <sstream>


std::string slurp(const char *filename)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	std::stringstream sstr;
	sstr << in.rdbuf();
	in.close();
	return sstr.str();
}

int main(){
	using namespace std;
	//using namespace boost;
	string s = slurp("input.txt");
	
	cout << "Input is:\n" << s << "\nTokens are:\n" ;

	//tokenizer<char_delimiters_separator<char> > tok(s);
	//tokenizer<char_separator > tok(s);

	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	boost::char_separator<char> sep{" \t\r\n", "\"", boost::drop_empty_tokens};
	//boost::char_separator<char> sep;
	tokenizer tok{s, sep};
	//for(tokenizer<char_delimiters_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
	//	cout << *beg << "\n";
	//}
	for(const auto &t : tok) cout << "Token: <" << t << ">\n";
}
