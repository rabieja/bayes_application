#include "decision_edge.h"
#include <iostream>

decision_edge::decision_edge(int id, tree_element* prev, tree_element* next, long double cost_of_additional_information, string description)
{
	this->id = id;
	this->prev = prev;
	this->next = next;
	this->description = description;
	this->type = "DECISION";
	this->winner = false;
	this->costs = cost_of_additional_information;
	this->helper = false;

}

bool decision_edge::is_node()
{
	return false;
}

bool decision_edge::is_edge()
{
	return true;
}

long double decision_edge::get_value()
{
	long double next_value = this->next->get_value();
	if (next_value == NULL) {
		this->next->set_value();
		next_value = this->next->get_value();
	}
	return next_value - this->costs;
}

long double decision_edge::get_probability()
{
	return 1.0;
}

long double decision_edge::get_costs()
{
	return costs;
}

void decision_edge::set_helper(bool helper_value)
{
	this->helper = helper_value;
}

string decision_edge::get_type()
{
	return "KrawêdŸ decyzyjna";
}
