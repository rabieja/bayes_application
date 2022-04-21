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
	bool validation(vector<tree_element*> &tree, map<int, edge*> &edges, map<int, node*> &nodes);
	bool validate_nodes(vector<tree_element*> &tree, map<int, node*> & nodes, map<int, edge*> &edges);
	bool validate_prev_element(map<int, node*> &nodes);
	bool validate_sum_probability(vector<tree_element*> &tree, map<int, node*> &nodes, map<int, edge*> &edges);
	int search_next_id(vector<tree_element*>& tree);
	void generate_helper_node(vector<tree_element*>& tree, map<int, node*>& nodes, map<int, edge*>& edges, node* node, double probability);
	bool validate_edges(map <int, edge*>);
	void set_value(tree_element*);
	tree_element* find_root(vector<tree_element*>);
	int create_png_graph(string);
	void create_dot_graph(string, vector<tree_element*>, map <int, edge*>,	map <int, node*>);
	void find_winner_trace(tree_element*);
};

