#pragma once

#include "edge.h"
#include "tree_element.h"

class chance_edge : public edge
{
public:
	chance_edge(int id, tree_element* prev, tree_element* next, double probability, string description);
	double probability;
	bool is_node();
	bool is_edge();
	double get_value();
};
