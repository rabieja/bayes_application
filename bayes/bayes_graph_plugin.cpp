#include "bayes_graph_plugin.h"

#include <gvc.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <iostream>
#include "edge.h"
#include "node.h"
#include "end_node.h"
#include "chance_node.h"
#include "decision_node.h"
#include "chance_edge.h"
#include "decision_edge.h"
#include "bayes_graph_engine.h"


using namespace std;


void bayes_graph_plugin::run()
{
	bayes_graph_engine bayes_graph_engine;
	map <int, edge*> edges_map;
	map <int, node*> nodes_map;
	vector <tree_element*> tree;


	string type = "", chance_node_description, decision_node_description, edge_description;
	char root;
	double end_node_value, cost_of_additional_information, probability;
	int size, id, prev_tree_element, next_tree_element;

	fstream data_file;
	data_file.open("dane.txt");

	while (!data_file.eof()) {
		data_file >> type >> size;
		if (data_file.eof()) {
			break;
		}
		if (type == "end_node") {
			for (int i = 0; i <= size - 1; i++) {
				data_file >> id >> end_node_value;
			//	cout << id << " " << end_node_value << endl;
				node* node_element = new end_node(id, end_node_value);
				nodes_map.insert(pair<int, node*>( id, node_element));
				tree.push_back(node_element);
			}continue;
		}else if(type == "chance_node") {
			for (int i = 0; i <= size - 1; i++) {
				data_file >> id;
				data_file.ignore(numeric_limits < streamsize >::max(), '\n');
				getline(data_file, chance_node_description);
				//cout << id << " " << chance_node_description << endl;
				node* node_element = new chance_node(id, chance_node_description);
				nodes_map.insert(pair<int, node*>(id, node_element));
				tree.push_back(node_element);
			}continue;
		}
		else if (type == "decision_node") {
			for (int i = 0; i <= size - 1; i++) {
				bool is_root = false;
				data_file >> id >> root;
				data_file.ignore(numeric_limits < streamsize >::max(), '\n');
				getline(data_file, decision_node_description);
			//	cout << id << " " << decision_node_description << " " << root << endl;
				if (root == 'y') {
					is_root = true;
				}
				else is_root = false;
				node* node_element = new decision_node(id, is_root, decision_node_description);

				nodes_map.insert(pair<int, node*>(id, node_element));
				tree.push_back(node_element);
			}continue;
		}
		else if (type == "edge_chance") {
			for (int i = 0; i <= size - 1; i++) {
				data_file >> id >> prev_tree_element >> next_tree_element >> probability;
				data_file.ignore(numeric_limits < streamsize >::max(), '\n');
				getline(data_file, edge_description);
			//	cout << id << " " << prev_tree_element << " " << next_tree_element << " " << probability << " " << edge_description << endl;
				edge* edge_element = new chance_edge(id, nodes_map.find(prev_tree_element)->second, 
					nodes_map.find(next_tree_element)->second, probability, edge_description);

				node* next = nodes_map.find(next_tree_element)->second;
				next->add_prev_element(edge_element);

				node* prev = nodes_map.find(prev_tree_element)->second;
				prev->add_next_element(edge_element);

				edges_map.insert(pair<int, edge*>(id, edge_element));
				tree.push_back(edge_element);
			}continue;
		}
		else if (type == "edge_decision") {
			for (int i = 0; i <= size - 1; i++) {
				data_file >> id >> prev_tree_element >> next_tree_element >> cost_of_additional_information;
				data_file.ignore(numeric_limits < streamsize >::max(), '\n');
				getline(data_file, edge_description);
			//	cout << id << " " << prev_tree_element << " " << next_tree_element << " " << edge_description << endl;
				edge* edge_element = new decision_edge(id, nodes_map.find(prev_tree_element)->second,
					nodes_map.find(next_tree_element)->second, cost_of_additional_information, edge_description);

				node* next = nodes_map.find(next_tree_element)->second;
				next->add_prev_element(edge_element);

				node* prev = nodes_map.find(prev_tree_element)->second;
				prev->add_next_element(edge_element);

				edges_map.insert(pair<int, edge*>(id, edge_element));
				tree.push_back(edge_element);
			}continue;
		}
	}
	data_file.close();

	if (bayes_graph_engine.validation(tree, edges_map, nodes_map)) {
		bayes_graph_engine.find_decision(tree);

		for (int i = 1; i <= 24; i++) {
				//	cout << nodes_map.find(i)->second->id << " " << nodes_map.find(i)->second->value << endl;
		}

		string file_name = "dane";

		bayes_graph_engine.create_dot_graph(file_name, tree, edges_map, nodes_map);
		bayes_graph_engine.create_png_graph(file_name);
	}
}

