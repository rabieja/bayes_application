#pragma once
#include <string>

using namespace std;

class tree_element
{
public:
	int id;
	string description;
	bool winner;
	bool helper;
	virtual bool is_node() = 0;
	virtual bool is_edge() = 0;
	virtual bool is_root() = 0;
	virtual void set_value() = 0;
	virtual double get_value() = 0;
	virtual void set_winner();
	virtual void set_helper(bool helper_value) = 0;
	virtual void find_winner() = 0;
	virtual tree_element * get_prev() = 0;
	bool get_winner();
};

