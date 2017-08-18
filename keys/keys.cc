#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

int main()
{
	cout << "q=quit, <RET> to see keys\n";

	while(1) {
		//cout << "Ready" << endl;
		char b = 0;
		size_t n = read(STDIN_FILENO, &b, 1);
		if(n<1) continue;
		if(b =='q') break;
		std::cout << int(b) << endl;
	}


	return 0;
}
