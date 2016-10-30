#include <functional>
#include <future>
#include <thread>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "yahoo.hpp"

/* This is a self-contained unit. You can use fetch_tickers as the functions to call
*/
// private
std::string run(const char* cmd)
{
	FILE *in;
	char buff[512];
	std::string result;

	if(!(in = popen(cmd, "r")))
		throw std::runtime_error("popen() failed");

	while(fgets(buff, sizeof(buff), in)!=NULL) result += buff;

	pclose(in);
	return result;
}

// private
std::string fetch_url(const std::string & url)
{
	std::string cmd = "curl -s -L " + url;
	//std::cout << cmd << "\n";
	return run(cmd.c_str());
	}

	// public
	std::vector<std::string> fetch_tickers(const std::vector<std::string>& tickers, std::string& usd)
	{
		std::string pre = "'http://download.finance.yahoo.com/d/quotes.csv?s=";	
		std::string url;


		// set up the futures
		std::future<std::string> fusd;
		url = pre + "USDGBP=X&f=nl1d1t1'";
		fusd = std::async(fetch_url, url);


		std::vector<std::future<std::string>> fs;
		for(auto& t:tickers) {
			std::string url = pre + t + "&f=sl1c1p2&e=.csv'";
			fs.push_back(std::async(fetch_url, url));
		}


		// now pull the results
		usd = fusd.get();
		std::vector<std::string> results;
		for(auto& f:fs) results.push_back(f.get());

		return results;
}


// public
bool operator<(const yahoo_t& a, const yahoo_t& b)
{
	return std::tie(a.ticker, a.dstamp, a.tstamp) < 
		std::tie(b.ticker, b.dstamp, b.tstamp);
}
bool has_ticker(const yahoo_ts& yahoos, const std::string& ticker)
{
	for(const auto& y:yahoos)
		if(y.ticker == ticker) return true;
	return false;
}
