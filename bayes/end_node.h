#pragma once
#include "node.h"
#include "tree_element.h"

class end_node : public node
{
public:
	end_node(int id, long double value);
	bool is_node();
	bool is_edge();
	bool is_root();
	void set_value() {};
	void find_winner();
	void set_helper(bool helper_value);
	string get_type();
};

