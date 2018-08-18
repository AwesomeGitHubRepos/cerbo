#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
//#include <regex>
#include <string>
//#include <tuple>
#include <vector>
using namespace std::string_literals;

using namespace std;

const char LRB = '(', RRB = ')';

int ch;
int yylval;;
string yytext;


void trace(string s)
{
	cout << "trace:" << s << "." << endl;
}

int nextch() { ch = getchar(); return ch; }

void eat_white() { while(isblank(ch)) nextch(); }



void label(string line)
{
}

void opcode(FILE* bout, string& line)
{
	int rx=0, ry=0;
	int32_t offset=0;
	//static regex re("([A-Z]+)(=.+)?");
	//smatch m;
	//regex_match(line, m, re);
	string opcode;
	line += ']'; // a convenient terminator
	int len = line.size();
	int i = 0;
	while(isalpha(line[i]))
    		opcode += line[i++];
	if(line[i++] == '=') {
		rx = line[i++] - '0';
		ry = line[i++] - '0';
		while(isdigit(line[i])) {
			//cout  << line[i] << "\n";
			offset = 10*offset + (line[i++] - '0');
			//cout << offset << "\n\n";
		}
	}
	
	trace("code:"s + opcode + ",rx:" + to_string(rx) + ",ry:" + to_string(ry) + ",off:" + to_string(offset));

	// generate binary
	static set<string> rn{{"ADD", "SUB", "MUL", "DIV", "STI", "LDI", "LDA", "LDR", "BAL"}};
	static set<string> off{{"STI", "LDI", "LDA", "BZE", "BNZ", "BRA"}};
	static vector<string> codes{{"HALT", "NOP", "TRAP", "ADD", "SUB", "MUL", "DIV", "STI", "LDI", "LDA", "LDR", "BZE", "BNZ", "BRA", "BAL"}};
	for(int i = 0; i< codes.size(); ++i) {
		if(codes[i] != opcode) continue;
		cout << "found opcode" << i << "\n";
		fwrite(&i, 1, sizeof(byte), bout);

		if(rn.find(opcode) != rn.end()) {
			cout << "found extended\n";
			byte b = (byte) (((rx & 0xff) << 4) + (ry & 0xff));
			fwrite(&b, 1, sizeof(byte), bout);
		}

		if(off.find(opcode) != off.end()) {
			byte barr[4];
			//(uint32_t) barr = offset;
			for(int bn=0; bn<4; ++bn) {
				byte b = (byte) (offset & 0xff);
			       	fwrite(&offset, 1, sizeof(byte), bout);
				offset = offset >> 8;
			}
		}
	}
	//static map 
}

int main()
{
	//ofstream bout;
	//bout.open("vasser.bin");	
	FILE* bout = fopen("vasser.bin", "wb");
	string line;
	while(getline(cin, line)) {
		//trace("line:" + line);
		if(line[0] == ':')
			label(line);
		else
			opcode(bout, line);
	}
	fclose(bout);

	/*
	while((ch = getchar()) != EOF)
		parse_line();
		*/

	return 0;
}
