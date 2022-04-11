#pragma once
#include "edge.h"
#include "tree_element.h"

using namespace std;

class decision_edge : public edge
{
public:
	decision_edge(int id, tree_element* prev, tree_element* next, string description);
	bool is_node();
	bool is_edge();
	double get_value();
	double get_probability();
	void set_winner();
};

