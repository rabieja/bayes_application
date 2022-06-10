#pragma once

#include "edge.h"
#include "tree_element.h"

class chance_edge : public edge
{
public:
	chance_edge(int id, tree_element* prev, tree_element* next, long double probability, string description);
	long double probability;
	bool is_node();
	bool is_edge();
	long double get_value();
	long double get_probability();
	long double get_costs();
	void set_helper(bool helper_value);
	string get_type();
};

