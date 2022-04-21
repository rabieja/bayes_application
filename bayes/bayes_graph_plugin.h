#pragma once
#ifndef bayes_graph_plugin_H
#define bayes_graph_plugin_H

#include "bayes_graph_engine.h"
#include <vector>

using namespace std;

class bayes_graph_plugin
{
public:
	bayes_graph_plugin();
	map <int, edge*> edges_map;
	map <int, node*> nodes_map;
	vector <tree_element*> tree;
	bayes_graph_engine engine;
	bool generate_from_file(string file_name);
	void generate_graph();
	void generate_decision_node(bool root);
	void generate_chance_node();
	void generate_end_node();
	void run();
};

#endif