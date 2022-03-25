#include "decision_node.h"
#include <string>
#include <vector>
#include "tree_element.h"
#include <limits>
#include <iostream>

const double lowest_double = -std::numeric_limits<double>::max();
using namespace std;

decision_node::decision_node(int id, bool root, string description)
{
	vector <tree_element*> next;
	this->id = id;
	this->root = root;
	this->description = description;
	this->next = next;
	this->type = "DECISION";
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
	cout << this->id << " " << this->next.size() << endl;
	for (int i = 0; i <= this->next.size() - 1; i++) {
		cout << this->next[i]->id << endl;
	}
	for (int i = 0; i <= this->next.size() - 1; i++) {
		cout << this->next[i]->get_value() << this->next[i]->is_edge() << endl;
		if (this->next[i]->is_edge()) {
			double value = this->next[i]->get_value();
			if (value == NULL) {
				cout << "aaa";
				this->next[i]->set_value();
				cout << "bbb";
				value = this->next[i]->get_value();
			}
			cout << this_value << " " << value << endl;
			this_value = max(this_value, value);
		}
	}this->value = this_value;
	return;
}
