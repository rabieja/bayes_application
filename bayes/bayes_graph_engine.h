#pragma once
#include <vector>
#include "tree_element.h"
#include <map>
#include "edge.h"
#include "node.h"


using namespace std;

class bayes_graph_engine
{
public:
	void find_decision(vector<tree_element*>);
	bool validation(map <int, edge*>, map <int, node*>);
	bool validate_nodes(map <int, node*>);
	bool validate_edges(map <int, edge*>);
	void set_value(tree_element*);
	tree_element* find_root(vector<tree_element*>);
	int create_png_graph(string);
	void create_dot_graph(string, vector<tree_element*>, map <int, edge*>,	map <int, node*>);
	void find_winner_trace(tree_element*);
};

