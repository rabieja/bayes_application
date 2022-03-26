#include "decision_edge.h"

decision_edge::decision_edge(int id, tree_element* prev, tree_element* next, string description)
{
	this->id = id;
	this->prev = prev;
	this->next = next;
	this->description = description;
	this->type = "DECISION";
}

bool decision_edge::is_node()
{
	return false;
}

bool decision_edge::is_edge()
{
	return true;
}

double decision_edge::get_value()
{
	return this->next->get_value();
}

double decision_edge::get_probability()
{
	return 1.0;
}
