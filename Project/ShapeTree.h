#pragma once
#include<iostream>
#include <vector>
#include "Vec3.h"
#include "GeometryShape.h"
#include "Geometry.h"
#include <functional>

class Node {
    Geometry shape;
    std::vector<Node *> children;
public:
    Node(Geometry& shape, std::vector<Node *> children){
        this->shape = shape;
        this->children = children;
    }
    Node(const Node &other){
        this->shape = other.shape;
        this->children = other.children;
    }
    Geometry & getShape(){
        return this->shape;
    }
    std::vector<Node *> getChildren(){
        return this->children;
    }
    void addChild(Node * child){
        this->children.push_back(child);
    }
};

class ShapeTree {
    Node *root;
	std::vector<Geometry> leafNodes;
public:
    ShapeTree();
    std::vector<Node *> applyRules(Node * current, std::vector<std::function<std::vector<Geometry>(Geometry)>> rules);
    void buildTree(std::vector<std::function<std::vector<Geometry>(Geometry)>> rules, Geometry& axiom);
	std::vector<Geometry> getLeafNodes();
	Node * getRoot();
};



