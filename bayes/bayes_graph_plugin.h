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
	void generate_tree_from_file();
	long double get_number();
	int get_int_number();
	void manual_generate_root();
	void manual_generate_decision_nodes();
	void manual_generate_chance_nodes();
	void manual_generate_end_nodes();
	void manuale_generate_edges();
	void manual_generation_tree();
	void save_graph_to_file();
	void generate_report();
	void run();
	void add_chance_edge(node* prev, node* next, string description);
	void add_decision_edge(node* prev, node* next, string description);
	void add_edge(int prev_id, int next_id);
	bool erase_node(map<int, node*>& nodes, vector<node*>& helper_nodes, int id);
	void show_all_nodes(map<int, node*>& nodes);
};

#endif