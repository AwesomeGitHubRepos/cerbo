#pragma once
#include <string>
#include <vector>

bool file_exists(const std::string &filename);
std::string slurp(const char *filename);
std::string slurp(const std::string filename);
void spit(const char *filename, const char *content);
void spit(const std::string& filename, const std::string& content);
void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void erase_all(std::string& str, const char c);

std::string nchars(char c, int num);
std::string pad_right(std::string s, int n);
std::string pad_left(std::string s, int n);
std::string format_num(double n, int dp);
std::string format_num(double n, int width, int dp);
std::string intercalate(std::string inter, std::vector<std::string> strings);

std::string expand_user(std::string path);

double bround(double x);
double round2(double x);
