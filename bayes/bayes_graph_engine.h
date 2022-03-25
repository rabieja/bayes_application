#pragma once
#include <vector>
#include "tree_element.h"

using namespace std;

static class bayes_graph_engine
{
public:
	void find_decision(vector<tree_element*>);
	void set_value(tree_element*);
	tree_element* find_root(vector<tree_element*>);
};

