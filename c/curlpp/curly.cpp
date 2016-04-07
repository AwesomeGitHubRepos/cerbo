#include <stdio.h>
#include <curl/curl.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <thread>
#include <memory>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::thread;
using std::unique_ptr;

class Curly {
public:
	Curly() {curl_global_init(CURL_GLOBAL_NOTHING);};
	~Curly() {curl_global_cleanup();};
};

class CurlyPull {
public:
	CurlyPull();
	~CurlyPull() {curl_easy_cleanup(curl);};
	void fetch() {thd = new thread (curl_easy_perform, curl);};
	void set_url(string url);
	void join() {thd->join();};
	string contents;
private:
	CURL *curl;
	string the_url;
	thread *thd = nullptr;
};


size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
   string *p_contents = (string *) userdata;
   p_contents->append(ptr, size * nmemb);
   return size* nmemb;   
} 

/* NB requires that your have called Curly at least once */
CurlyPull::CurlyPull()
{
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &contents);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
}
void CurlyPull::set_url(string url)
{
	the_url = url;
	curl_easy_setopt(curl, CURLOPT_URL, the_url.c_str());
}

	
vector<string> fetch_urls(const vector<string> &urls)
{
	Curly curly;
	int len = urls.size(), i;
	CurlyPull cparr[len];
	for(i=0; i<len; i++) cparr[i].set_url(urls[i]);
	for(i=0; i<len; i++) cparr[i].fetch();
	for(i=0; i<len; i++) cparr[i].join();
	
	vector<string> contentss;
	for(i=0; i<len; i++) contentss.push_back(cparr[i].contents);
	return contentss;
	
}


