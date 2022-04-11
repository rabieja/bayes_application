#pragma once
#include "tree_element.h"
#include <string>

using namespace std;

class edge : public tree_element
{
public:
	tree_element * prev;
	tree_element * next;
	bool winner;
	virtual bool is_node() = 0;
	virtual bool is_edge() = 0;
	bool is_root();
	void set_value();
	virtual double get_value() = 0;
	virtual double get_probability() = 0;
	virtual void set_winner() = 0;
	string type;
};

