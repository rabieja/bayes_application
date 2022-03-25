#include "bayes_graph_engine.h"
#include "node.h"
#include <iostream>

using namespace std;

void bayes_graph_engine::find_decision(vector<tree_element*> tree)
{
	tree_element* root = find_root(tree);
	if (root != NULL) {
		set_value(root);
	}

}

void bayes_graph_engine::set_value(tree_element* element)
{
	if (element->is_node()) {
		element->set_value();
	}
}

tree_element* bayes_graph_engine::find_root(vector<tree_element*> tree)
{
	for (int i = 0; i <= tree.size() - 1; i++) {
		if (tree[i]->is_node() && tree[i]->is_root()) {
			return tree[i];
		}
	}
	return NULL;
}

