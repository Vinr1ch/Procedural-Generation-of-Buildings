#pragma once
#include<iostream>
#include<vector>
#include<functional>
#include<random>
#include <math.h>  
#include"Geometry.h"


class Parser {
	std::string fileName; // file with rules
	std::vector<std::string> readLines(std::string rulesFile); // read lines from file
	std::vector<std::string> splitString(std::string str, char delimeter, char delimeter2); 
	bool startsWith(std::string str, std::string prefix);
	GeometryShape stringToType(std::string str);
	std::function<std::vector<Geometry>(Geometry)> stringToRule(std::string string); 
	std::vector<std::function<float()>> parseArguments(std::string token);
	std::vector<std::string> separateLine(std::string token);
public:
	Parser(std::string rulesFile);
	std::vector<std::function<std::vector<Geometry>(Geometry)>> parseRules(); 
};

