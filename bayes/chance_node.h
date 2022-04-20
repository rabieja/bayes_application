#pragma once
#include "node.h"
#include "tree_element.h"
#include <vector>
#include <string>

using namespace std;

class chance_node : public node
{
public:
	chance_node(int id, string description);
	bool is_node();
	bool is_edge();
	bool is_root();
	void set_value();
	void find_winner();
};

