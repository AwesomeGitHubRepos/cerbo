#include <string>
#include <vector>
#include <iostream>

#include "curly.h"


using std::string;
using std::vector;
using std::cout;
using std::endl;

string ticker_to_url(string ticker)
{
	string url = "http://download.finance.yahoo.com/d/quotes.csv?s=";
	url += ticker + "&f=sl1c1p2&e=.csv";
	return url;
}
vector<string> tickers_to_urls(vector<string> tickers)
{
	vector<string> urls;
	for(auto t:tickers) urls.push_back(ticker_to_url(t));
	return urls;
}
int main(int argc, char **argv)
{

	string url;
	auto tickers = vector<string> { "AZN.L", "BOGUS", "ULVR.L", "VOD.L" };
	auto urls = tickers_to_urls(tickers);
	
	auto contents = fetch_urls(urls);
	for(auto c:contents) {
		int len = c.size();
		if(len>0 && c[len-1] == '\n') c.erase(len-1);
		cout << c << endl;
	}	
	
	puts("Finished");
	
	return 0;
}
