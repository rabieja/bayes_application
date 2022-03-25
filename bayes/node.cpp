#include "node.h"
#include <vector>
#include "tree_element.h"
#include <iostream>

using namespace std;

void node::add_prev_element(tree_element* prev)
{
	this->prev = prev;
}

double node::get_value()
{
	return this->value;
}

void node::add_next_element(tree_element* element)
{
	this->next.push_back(element);
}
