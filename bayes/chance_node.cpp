#include "chance_node.h"
#include <vector>
#include "tree_element.h"
#include <iostream>

using namespace std;

chance_node::chance_node(int id, string description)
{
	vector <tree_element*> next;
	this->id = id;
	this->description = description;
	this->next = next;
	this->type = "CHANCE";
}

bool chance_node::is_node()
{
	return true;
}

bool chance_node::is_edge()
{
	return false;
}

bool chance_node::is_root()
{
	return false;
}

void chance_node::set_value()
{
	double this_value = 0;
	for (int i = 0; i <= this->next.size() - 1; i++) {
		if (this->next[i]->is_edge()) {
			double value = this->next[i]->get_value();
			if (value == NULL) {
				this->next[i]->set_value();
				value = this->next[i]->get_value();
			}this_value += value;
		}
	}this->value = this_value;
	return;
}
