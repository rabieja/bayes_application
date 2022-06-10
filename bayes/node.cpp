#include "node.h"
#include <vector>
#include "tree_element.h"
#include <iostream>

using namespace std;

void node::add_prev_element(edge* prev)
{
	this->prev = prev;
}

long double node::get_value()
{
	return this->value;
}

void node::add_next_element(edge* element)
{
	this->next.push_back(element);
}
