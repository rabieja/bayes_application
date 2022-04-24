#include "decision_node.h"
#include <string>
#include <vector>
#include "tree_element.h"
#include "edge.h"
#include <limits>
#include <iostream>
#include "decision_edge.h"

const double lowest_double = -std::numeric_limits<double>::max();
using namespace std;

decision_node::decision_node(int id, bool root, string description)
{
	vector <edge*> next;
	this->id = id;
	this->root = root;
	this->description = description;
	this->next = next;
	this->type = "DECISION";
	this->helper = false;

}

bool decision_node::is_node()
{
	return true;
}

bool decision_node::is_edge()
{
	return false;
}

bool decision_node::is_root()
{
	return this->root;
}

void decision_node::set_value()
{
	double this_value = lowest_double;
	for (int i = 0; i <= this->next.size() - 1; i++) {
		if (this->next[i]->is_edge()) {
			double value = this->next[i]->get_value();
			if (value == NULL) {
				this->next[i]->set_value();
				value = this->next[i]->get_value();
			}

			this_value = max(this_value, value);
		}
	}this->value = this_value;
	return;
}

void decision_node::find_winner() {

	vector <tree_element*> next;
	double this_value = lowest_double;

	for (int i = 0; i <= this->next.size() - 1; i++) {
		if (this->next[i]->is_edge()) {
			double value = this->next[i]->get_value();

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

void decision_node::set_helper(bool helper_value)
{
	this->helper = helper_value;
}
