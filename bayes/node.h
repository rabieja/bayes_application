#pragma once
#include "tree_element.h"
#include "edge.h"
#include <vector>
#include <string>

using namespace std;

class node : public tree_element
{
public:
	string type;
	double value;
	edge* prev;
	vector <edge* > next;
	void add_next_element(edge*);
	void add_prev_element(edge *);
	virtual bool is_node() = 0;
	virtual bool is_edge() = 0;
	virtual bool is_root() = 0;
	virtual void set_value() = 0;
	double get_value();
	virtual void find_winner() = 0;
	virtual void set_helper(bool helper_value) = 0;
	tree_element* get_prev() { return prev; }
	virtual string get_type() = 0;
};

