#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include"Model/Node.h"
#include"Model/Map.h"
#include <unordered_map>

class PathFinder
{
public:
	PathFinder();
	~PathFinder();
	double manhattan_distance(int, int, int, int);
	std::vector<Node> path(Node, Node, Map);
	static std::vector<Node> reconstruct_path(std::unordered_map<Node, Node, Node::NodeHash>, Node);
	static bool is_valid_neighbour(Node);
	static bool findNode(Node*, std::vector<Node*>);	
};



