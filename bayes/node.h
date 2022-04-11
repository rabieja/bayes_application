#pragma once
#include "tree_element.h"
#include <vector>
#include <string>

using namespace std;

class node : public tree_element
{
public:
	string type;
	double value;
	tree_element* prev;
	vector <tree_element* > next;
	void add_next_element(tree_element*);
	void add_prev_element(tree_element *);
	virtual bool is_node() = 0;
	virtual bool is_edge() = 0;
	virtual bool is_root() = 0;
	virtual void set_value() = 0;
	double get_value();
	virtual void set_winner() = 0;
};

