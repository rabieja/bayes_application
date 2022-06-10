#include "chance_edge.h"
#include <iostream>

chance_edge::chance_edge(int id, tree_element* prev, tree_element* next, long double probability, string description)
{
	this->id = id;
	this->prev = prev;
	this->next = next;
	this->description = description;
	this->probability = probability;
	this->type = "CHANCE";
	this->winner = false;
	this->helper = false;
}

bool chance_edge::is_node()
{
	return false;
}

bool chance_edge::is_edge()
{
	return true;
}

long double chance_edge::get_value()
{
	long double next_value = this->next->get_value();
	if (next_value == NULL) {
		this->next->set_value();
		next_value = this->next->get_value();
	}
	return next_value * this->probability;
}

long double chance_edge::get_probability()
{
	return probability;
}

long double chance_edge::get_costs()
{
	return 0.0;
}

void chance_edge::set_helper(bool helper_value)
{
	this->helper = helper_value;
}

string chance_edge::get_type()
{
	return "KrawêdŸ losowa";
}
