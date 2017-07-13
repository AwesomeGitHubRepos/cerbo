#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "inputs.h"

std::string rootdir();
std::string workdir();
std::string sndir(int n);


std::string sn(int n, const std::string& name);
std::string s0(const std::string& name);
std::string s1(const std::string& name);
std::string s2(const std::string& name);
std::string s3(const std::string& name);
//typedef std::map<std::string, std::vector<std::string> > msvs_t;


void conv(double& d, std::string s);
void conv(std::string& dest, std::string src);

template<typename T>
void print(const std::vector<T>& xs)
{
	for(auto& x: xs) std::cout << x << " ";
	std::cout << std::endl;
}

