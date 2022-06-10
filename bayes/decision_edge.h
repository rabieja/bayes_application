#pragma once
#include "edge.h"
#include "tree_element.h"

using namespace std;

class decision_edge : public edge
{
public:
	decision_edge(int id, tree_element* prev, tree_element* next, long double additional_information_cost, string description);
	bool is_node();
	bool is_edge();
	long double costs;
	long double get_value();
	long double get_probability();
	long double get_costs();
	void set_helper(bool helper_value);
	string get_type();

};

