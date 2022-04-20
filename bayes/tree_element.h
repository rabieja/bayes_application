#pragma once
#include <string>

using namespace std;

class tree_element
{
public:
	int id;
	string description;
	bool winner;
	virtual bool is_node() = 0;
	virtual bool is_edge() = 0;
	virtual bool is_root() = 0;
	virtual void set_value() = 0;
	virtual double get_value() = 0;
	virtual void set_winner();
	virtual void find_winner() = 0;
};

