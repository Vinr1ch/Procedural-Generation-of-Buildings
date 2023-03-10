#include <iterator>
#include <algorithm> 
#include <stack>
#include <string>
#include <fstream>
#include "Parser.h"

//random number generator objects
std::random_device rd;
std::mt19937 mt(rd());
std::stack<Geometry>  *processing = new std::stack<Geometry>(); //shape being processed

Parser::Parser(std::string rulesFile) {
	this->fileName = rulesFile;
}

/**Reads all line in a file into vector*/
std::vector<std::string> Parser::readLines(std::string rulesFile) {
	std::vector<std::string> linesFromFile;
	std::ifstream dict_file(rulesFile);
	std::string line;
	while (std::getline(dict_file, line))
		if(line[0]!='#') // ignoring comment lines
			linesFromFile.push_back(line);
	return linesFromFile;
}

std::vector<std::string> Parser::splitString(std::string str, char delimeter, char delimeter2) {
	std::vector<std::string> words;
	//split string
	unsigned int last_pos = 0;
	for (unsigned int i = 0; i < str.length(); i++)
		if (str[i] == delimeter || str[i] == delimeter2) {
			words.push_back(str.substr(last_pos, i - last_pos));
			last_pos = i + 1;
		}
	if (last_pos != str.length())
		words.push_back(str.substr(last_pos, str.length() - last_pos));
	return words;
}

bool Parser::startsWith(std::string str, std::string prefix){
	return str.compare(0, prefix.length(), prefix) == 0;
}

GeometryShape Parser::stringToType(std::string str) {
	if (!str.compare("cube"))
		return CUBE;
	else if (!str.compare("cylinder"))
		return CYLINDER;
	else if (!str.compare("scope"))
		return SCOPE;
	else if (!str.compare("plain"))
		return PLAIN;
	else throw "Can't parse string to shape.";
}

std::function<std::vector<Geometry>(Geometry)> Parser::stringToRule(std::string string){
	auto tokens = splitString(string, ' ', ' '); // oprations to be performed
	std::string sourceScope = tokens[0]; //shape the rule will be applied
	std::vector<Geometry> *result = new std::vector<Geometry>() ; //shapes being return to build tree
	
	std::function<void(Geometry)> rule = [=](Geometry x) { (*processing).push(x); };  //initial rule

	for (int i = 2; i < tokens.size(); i++) { // i=2 skipping sourceScope and '=>'
		
		if (startsWith(tokens[i],"T")) { //Scope translation
			auto args = parseArguments(tokens[i]);
		 	rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				currentShape.move(Vec3(args[0](), args[1](), args[2]()));
			};
		}

		else if (startsWith(tokens[i], "Subdiv")) { // divide scope in smaller scopes
			auto args = parseArguments(tokens[i]);
			auto parameters = separateLine(tokens[i]);
			int axis = round(args[0]());
			rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				std::vector<float> ratios;
				for (int i = 1; i < args.size(); ratios.push_back(args[i++]())); //parsing ratios
				auto newShapes = currentShape.split(axis, ratios, parameters);
				std::copy(newShapes.begin(), newShapes.end(), std::back_inserter((*result))); //save new shape to result
			};
		}

		else if (startsWith(tokens[i], "S")) { // Set new scope size
			auto args = parseArguments(tokens[i]);
			rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				currentShape.setSize(Vec3(args[0](), args[1](), args[2]()));
			};
		}

		else if (startsWith(tokens[i], "[")) { // push current scope to the stack
			rule = [=](Geometry x) {
				rule(x);
				(*processing).push(x);
				(*processing).push(Geometry(x.getName(),x.getPosition(), x.getSize(), x.getType()));
			};
		}

		else if (startsWith(tokens[i], "]")) { // // pop current scope from the stack
			rule = [=](Geometry x) {
				rule(x);
				(*processing).pop();
			};
		}

		else if (startsWith(tokens[i], "I")) { // Instantiate a figure to draw
			auto shape = stringToType(splitString(tokens[i], '(', ')')[1]);
			auto material = separateLine(tokens[i])[0]; // texture to draw
			rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				currentShape.setName(material); 
				currentShape.setType(shape);
				(*result).push_back(currentShape);
			};
		}

		else if (startsWith(tokens[i], "Repeat")) { // splitting scope into the same objects
			auto args = parseArguments(tokens[i]);
			auto newShapeName = separateLine(tokens[i])[0];
			int axis = round(args[0]());
			rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				auto newShapes = currentShape.repeat(axis, round(args[1]()), newShapeName);
				std::copy(newShapes.begin(), newShapes.end(), std::back_inserter((*result))); //save new shape to result
			};
		}
		else if (startsWith(tokens[i], "RealRepeat")) { // repeat shape along axis
			auto args = parseArguments(tokens[i]);
			auto parameters = separateLine(tokens[i]);
			int axis = round(args[0]());
			rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				auto newShapes = currentShape.realRepeat(axis, round(args[1]()), parameters[0]);
				std::copy(newShapes.begin(), newShapes.end(), std::back_inserter((*result))); //save new shape to result
			};
		}
		else if (startsWith(tokens[i], "R")) { // rotating around axes
			auto args = parseArguments(tokens[i]);
			rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				currentShape.rotate(Vec3(args[0](), args[1](), args[2]()));
			};
		}

		else if (startsWith(tokens[i], "Comp")) { // split scope into  plains
			auto shape = splitString(tokens[i], '(', ')')[1];
			auto parameters = separateLine(tokens[i]);
			rule = [=](Geometry x) {
				rule(x);
				Geometry& currentShape = (*processing).top();
				auto newShapes = currentShape.componentSplit (shape,parameters);
				std::copy(newShapes.begin(), newShapes.end(), std::back_inserter((*result))); //save new shape to result
			};
		}
		//else throw "Parser: no such command";
	}

	return [=](Geometry shape){
		(*result).clear();
		if(sourceScope.compare(shape.getName())==0) // apply rule if name in rule head matches
			rule(shape);
		return (*result);};
}

std::vector<std::function<float()>> Parser::parseArguments(std::string token){
	auto args = splitString(splitString(token, '(', ')')[1], ',', ',');
	std::vector<std::function<float()>> funs;
	for (int k = 0; k < args.size(); k++) {
		if (startsWith(args[k],"rnd")) {
			auto rArgs = splitString(splitString(args[k], '<', '>')[1], '-', '-');
			std::uniform_real_distribution<double> dist(std::stof(rArgs[0]), std::stof(rArgs[1]));
			funs.push_back([=] { return dist(mt); });
		}
		else if (startsWith(args[k], "r")) {
			int axis = funs[0]();
			std::string number = args[k].substr(1, args[k].length());
			funs.push_back([=] {
				Geometry& currentShape = (*processing).top();
				float size = currentShape.getSize().getElement(axis);;
				return std::stof(number)*size; });
		}
		else {
			float value = std::stof(args[k]);
			funs.push_back([=] { return value; });
		}
	}
	return funs;
}

std::vector<std::string> Parser::separateLine(std::string token){
	return splitString(splitString(token, '{', '}')[1], ',', ',');
}

std::vector<std::function<std::vector<Geometry>(Geometry)>> Parser::parseRules() {
	std::vector<std::function<std::vector<Geometry>(Geometry)>> rules;
	auto lines = readLines(this->fileName);
	for (auto line : lines)
		rules.push_back(stringToRule(line));
	return rules;
}
