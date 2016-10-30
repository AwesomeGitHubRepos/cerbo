#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

//#include <boost/filesystem.hpp>
//namespace fsys = boost::filesystem;

#include "common.hpp"
#include "reusable.hpp"

using namespace std;


void mkdir(const string& path)
{
	struct stat st;
	// need to convert from a const string to a non-const char*
	vector<char> p1(path.begin(), path.end());
	p1.push_back(0);
	char* dir = &p1[0];
	if(stat(dir, &st) !=0)
		mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

string rootdir()
{
	string root = expand_user("~/.mcacc");
	static bool created = false;
	if(!created) {
		//fsys::create_directory(root);
		mkdir(root);
		string s = root + "/work";
		//fsys::create_directory(s);
		mkdir(s);
		//fsys::create_directory(s + "/s1");
		mkdir(s+"/s1");
		//fsys::create_directory(s + "/s2");
		mkdir(s+"/s2");
		//fsys::create_directory(s + "/s3");
		mkdir(s+"/s3");
		//fsys::create_directory(root + "/yahoo");
		mkdir(root+"/yahoo");
		created = true;
	}
	return root;
}

std::string workdir() { return rootdir() + "/work" ; }


std::string sndir(int n)
{
	return workdir() + "/s" + to_string(n);
}

// create a filename that is based in the stage N directory
void sn(int n, const char *name, string &outname) 
{
	//outname = workdir();
	//outname +=  "/s" +  to_string(n)  + "/" + name;
	outname = sndir(n) + "/" + name;
}

void s1(const char *name, string &outname) { sn(1, name, outname);}
void s2(const char *name, string &outname) { sn(2, name, outname);}
void s3(const char *name, string &outname) { sn(3, name, outname);}
std::string s3(const std::string& name) { string outname; s3(name.c_str(), outname); return outname; }

typedef map<string, vector<string> > msvs_t;



void conv(double& d, std::string s) { d = stod(s); }
void conv(std::string& dest, std::string src) { dest = src; }
