#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

char match1(char inp, char reg)
{

	char s[] = { inp , 0 };
	//string s("g");
	const string expr = string("\\") + ((char) reg);
	//cout << expr << s <<" ";
	regex r(expr);
	//regex r("\\S");

	cmatch cm;
	regex_match(s, cm, r);
	//cout << cm.size();
	return cm.size() ? reg : ' ';
	//cout << cm.size() << endl;

}

char match(char inp, char reg)
{
	try {
		return match1(inp, reg);
	}
	catch(...)
	{
		return ' ';
	}
}

int main()
{
	const vector<char> rs = { 'a', 'b', 'd', 'D', 'l', 'p', 's', 'S', 'u', 'w', 'W', 'x' };
	for(int c=0; c<256; c++) {
		cout <<c << " " << (char) c << " " ;
		for(char r:rs) cout << match(c, r);
		cout <<endl;
	}

	return 0;
}
