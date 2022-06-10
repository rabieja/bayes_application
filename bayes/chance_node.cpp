#include "chance_node.h"
#include <vector>
#include "tree_element.h"
#include "edge.h"
#include <iostream>

const long double lowest_double = -std::numeric_limits<long double>::max();
using namespace std;

chance_node::chance_node(int id, string description)
{
	vector <edge*> next;
	this->id = id;
	this->description = description;
	this->next = next;
	this->type = "CHANCE";
	this->helper = false;

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
	long double this_value = 0;
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

void chance_node::find_winner()
{
	vector <tree_element*> next;
	long double this_value = lowest_double;

	for (int i = 0; i <= this->next.size() - 1; i++) {
		if (this->next[i]->is_edge()) {
			long double value = this->next[i]->get_value();

			if (value == this_value) {
				next.push_back(this->next[i]);
			}
			else if (value > this_value) {
				next.clear();
				next.push_back(this->next[i]);
			}
			this_value = max(this_value, value);
		}
	}
	for (int i = 0; i <= next.size() - 1; i++) {
		next[i]->set_winner();
		next[i]->find_winner();
	}

	return;

}

void chance_node::set_helper(bool helper_value)
{
	this->helper = helper_value;
}

string chance_node::get_type()
{
	return "Wêze³ losowy";
}
