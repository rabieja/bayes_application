#pragma once
#include "node.h"
#include "tree_element.h"
#include <vector>
#include <string>

using namespace std;

class decision_node : public node
{
public:
	decision_node(int id, bool root, string description);
	bool root;
	bool is_node();
	bool is_edge();
	bool is_root();
	void set_value();
	void find_winner();
	void set_helper(bool helper_value);
	string get_type();

};

