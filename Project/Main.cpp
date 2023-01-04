#include"Parser.h"
#include "Geometry.h"
#include "Visualizer.h"
#include "ShapeTree.h"

using namespace std;


int main(int argc, char** argv) {
	Parser parser("rules.txt");
	auto rules = parser.parseRules();
	ShapeTree tree;
	tree.buildTree(rules, Geometry("main", Vec3(0,0,0), Vec3(40, 120, 80), SCOPE));
	auto nodes = tree.getLeafNodes();

	drawShapes(nodes, argc, argv);
	
	return 0;
}