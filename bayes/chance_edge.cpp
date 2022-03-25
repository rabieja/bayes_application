#include "chance_edge.h"
#include <iostream>

chance_edge::chance_edge(int id, tree_element* prev, tree_element* next, double probability, string description)
{
	this->id = id;
	this->prev = prev;
	this->next = next;
	this->description = description;
	this->probability = probability;
}

bool chance_edge::is_node()
{
	return false;
}

bool chance_edge::is_edge()
{
	return true;
}

double chance_edge::get_value()
{
	double next_value = this->next->get_value();
	if (next_value == NULL) {
		this->next->set_value();
		next_value = this->next->get_value();
	}
	return next_value * this->probability;
}
