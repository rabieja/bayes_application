﻿#include "bayes_graph_engine.h"
#include "node.h"
#include "edge.h"
#include "end_node.h"
#include <iostream>
#include <gvc.h>
#include <string>
#include <map>
#include <fstream>
#include "chance_edge.h"
#include <locale.h>
#include <locale>
#include <codecvt>
#include <iomanip> 

using namespace std;

void bayes_graph_engine::find_decision(vector<tree_element*> tree)
{
	tree_element* root = find_root(tree);
	if (root != NULL) {
		set_value(root);
		find_winner_trace(root);
	}

}

bool bayes_graph_engine::validation(vector<tree_element*>& tree, map<int, edge*>& edges, map<int, node*>& nodes)
{
	if (!validate_nodes(tree, nodes, edges) || !validate_edges(edges)) {
		return false;
	}

	return true;
}

bool bayes_graph_engine::validate_nodes(vector<tree_element*>& tree, map<int, node*>& nodes, map<int, edge*>& edges)
{
	return validate_prev_element(nodes) &&  validate_sum_probability(tree, nodes, edges) && validate_next_elements(nodes);
}

bool bayes_graph_engine::validate_next_elements(map<int, node*>& nodes) {
	ofstream logs("logs.txt", ios::out | ios::app);

	string description = "";
	bool result = true;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		description = iter->second->description;
		if (iter->second->type != "END" && iter->second->next.empty()) {
			cout << "Wprowadzone dane sa nieprawidłowe, węzeł \" " << description << " \" nie posiada następników." << endl;
			logs << "Wprowadzone dane sa nieprawidłowe, węzeł \" " << description << " \" nie posiada następników." << endl;

			result = false;
		}
	}
	return result;
}

bool bayes_graph_engine::validate_prev_element(map<int, node*>& nodes) {
	ofstream logs("logs.txt", ios::out | ios::app);

	string description = "";
	bool result = true;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		description = iter->second->description;
		if (!iter->second->is_root() && iter->second->prev == NULL) {
			cout << "Wprowadzone dane sa nieprawidłowe, węzeł \" " << description << " \" nie posiada poprzednika." << endl;
			logs << "Wprowadzone dane sa nieprawidłowe, węzeł \" " << description << " \" nie posiada poprzednika." << endl;

			result = false;
		}
	}
	return result;
}
void bayes_graph_engine::generate_helper_node(vector<tree_element*>& tree, map<int, node*>& nodes, map<int, edge*>& edges, node* node_prev, long double probability) {

	int id = search_next_id(tree);

	node* node_element = new end_node(id, 0);
	node_element->set_helper(true);
	nodes.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);
	id++;
	edge* edge_element = new chance_edge(id, node_prev, node_element, probability, "");
	edge_element->set_helper(true);
	edges.insert(pair<int, edge*>(id, edge_element));
	tree.push_back(edge_element);

	node_element->add_prev_element(edge_element);
	node_prev->add_next_element(edge_element);
}
bool bayes_graph_engine::validate_sum_probability(vector<tree_element*>& tree, map<int, node*>& nodes, map<int, edge*>& edges) {
	ofstream logs("logs.txt", ios::out | ios::app);

	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		if (iter->second->type == "CHANCE") {

			long double sum = 0;
			if (!iter->second->next.empty()) {
				for (int i = 0; i <= iter->second->next.size() - 1; i++) {
					if (iter->second->next[i]->type != "CHANCE") {
						cout << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << iter->second->next[i]->description << " \" jest złego typu. Wymagany typ to gałąź losowa." << endl;
						logs << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << iter->second->next[i]->description << " \" jest złego typu. Wymagany typ to gałąź losowa." << endl;
						return false;
					}
					else sum += iter->second->next[i]->get_probability();
				}
			}
			if (sum > 1) {
				cout << "Wprowadzone dane sa nieprawidłowe, suma prawdopodobieństw w węźle \" " << iter->second->description << " \" jest większa od 1.0." << endl;
				logs << "Wprowadzone dane sa nieprawidłowe, suma prawdopodobieństw w węźle \" " << iter->second->description << " \" jest większa od 1.0." << endl;
				return false;
			}
			else if (sum < 1) {
				string answer = "";
				cout << "Wprowadzone dane sa nieprawidłowe, suma prawdopodobieństw w węźle \" " << iter->second->description << " \" jest mniejsza od 1.0." << endl;
				logs << "Wprowadzone dane sa nieprawidłowe, suma prawdopodobieństw w węźle \" " << iter->second->description << " \" jest mniejsza od 1.0." << endl;

				while (answer != "tak" && answer != "nie" && answer != "TAK" && answer != "NIE") {
					cout << "Czy chcesz zredukować błąd dodając węzeł pomocniczy, którego wartość monetarna jest równa 0? (tak/nie)" << endl;
					logs << "Czy chcesz zredukować błąd dodając węzeł pomocniczy, którego wartość monetarna jest równa 0? (tak/nie)" << endl;

					getline(cin, answer);
					logs << answer << endl;
				}
				if (answer == "tak" || answer == "Tak" || answer == "TAK") {
					generate_helper_node(tree, nodes, edges, iter->second, 1-sum);
				}
				else return false;
			}
		}
		else if (iter->second->type == "DECISION") {
			if (!iter->second->next.empty()) {
				for (int i = 0; i <= iter->second->next.size() - 1; i++) {
					if (iter->second->next[i]->type != "DECISION") {
						cout << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << iter->second->next[i]->description << " \" jest złego typu. Wymagany typ to gałąź decyzyjna." << endl;
						logs << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << iter->second->next[i]->description << " \" jest złego typu. Wymagany typ to gałąź decyzyjna." << endl;
						return false;
					}
				}
			}
		}
	}
	return true;
}

int bayes_graph_engine::search_next_id(vector<tree_element*>& tree) {
	int id=0;
	for (int i = 0; i <= tree.size() - 1; i++) {
		id = max(id, tree[i]->id);
	}return id++;
}

bool bayes_graph_engine::validate_edges(map<int, edge*> edges)
{
	ofstream logs("logs.txt", ios::out | ios::app);

	bool result = true;
	string description = "";
	for (map<int, edge*>::iterator iter = edges.begin(); iter != edges.end(); ++iter) {
		description = iter->second->description;
		if (iter->second->prev == NULL) {
			cout << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << description << " \" nie posiada poprzednika." << endl;
			logs << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << description << " \" nie posiada poprzednika." << endl;

			result = false;
		}
		if (iter->second->next == NULL) {
			cout << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << description << " \" nie posiada następnika." << endl;
			logs << "Wprowadzone dane sa nieprawidłowe, gałąź \" " << description << " \" nie posiada następnika." << endl;

			result = false;
		}
	}
	return result;
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

int bayes_graph_engine::create_png_graph(string file_name)
{
	GVC_t* gvc;
	Agraph_t* graph;
	FILE* filep;
	gvc = gvContext();
	filep = fopen((file_name + "/tree.dot").c_str(), "r");
	graph = agread(filep, 0);
	gvLayout(gvc, graph, "dot");
	gvRenderFilename(gvc, graph, "png", (file_name + "/tree.png").c_str());

	gvFreeLayout(gvc, graph);
	agclose(graph);
	return gvFreeContext(gvc);
}

void bayes_graph_engine::create_dot_graph(string file_name, vector<tree_element*> tree, map<int, edge*> edges, map<int, node*> nodes)
{
	locale lc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wofstream save(file_name + "/tree.dot");
	save.imbue(std::locale(".utf8"));

	save << "strict graph{" << endl;
	save << "rankdir = LR; " << endl;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {

		if (iter->second->type == "DECISION") {
			string s = iter->second->description;
			std::locale loc(".1250");
			auto& cctv = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
			std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter(&cctv);
			std::wstring wide = converter.from_bytes(s);

			save << iter->second->id << "[shape=square label = \"" << wide << "\" xlabel = \" " << iter->second->value << "\" ] " << endl;
		}
		else if (iter->second->type == "CHANCE") {
			string s = iter->second->description;
			std::locale loc(".1250");
			auto& cctv = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
			std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter(&cctv);
			std::wstring wide = converter.from_bytes(s);

			save << iter->second->id << "[shape=circle label = \"" << wide << "\" xlabel=\" " <<  iter->second->value << "\"] " << endl;
		}
		else {
			save << iter->second->id << "[shape=none label=\" " << iter->second->value << "\"] " << endl;
		}
		if (iter->second->winner) {
			save << "[color = red];" << endl;
		}
		else if (iter->second->helper) {
			save << "[color = grey];" << endl;
		}else save << "[color = black];" << endl;
	}
	for (map<int, edge*>::iterator iter = edges.begin(); iter != edges.end(); ++iter) {

		string s = iter->second->description;
		std::locale loc(".1250");
		auto& cctv = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
		std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter(&cctv);
		std::wstring wide = converter.from_bytes(s);

		if (iter->second->type == "DECISION") {
			save << iter->second->prev->id << " -- " << iter->second->next->id << "[label=\"" << wide;
			if (iter->second->get_costs() != 0) {
				save << " koszt: " << iter->second->get_costs() << "\"] ";
			}else save << "\"] ";

		}
		else if (iter->second->type == "CHANCE") {
			save << iter->second->prev->id << " -- " << iter->second->next->id << "[label=\"" << wide << " P = " <<  
					iter->second->get_probability() << "\"] ";
		}
		if (iter->second->winner) {
			save << "[color = red];" << endl;
		}
		else if (iter->second->helper) {
			save << "[color = grey];" << endl;
		}else save << "[color = black];" << endl;
	}
	save << "}";

	save.close();
}

void bayes_graph_engine::find_winner_trace(tree_element* element)
{
	if (element->is_node() && element->is_root()) {
		element->set_winner();
		element->find_winner();
	}
}

void bayes_graph_engine::save_graph_to_file(map < int, edge *> &edges_map, map < int, node*> &nodes_map, vector <tree_element*> &tree, string file_name) {

	vector <node*> end_nodes;
	vector <node*> chance_nodes;
	vector <node*> decision_nodes;
	vector <edge*> chance_edges;
	vector <edge*> decision_edges;

	for (map<int, node*>::iterator iter = nodes_map.begin(); iter != nodes_map.end(); ++iter) {
		if (iter->second->type == "END") {
			end_nodes.push_back(iter->second);
		}
		else if (iter->second->type == "CHANCE") {
			chance_nodes.push_back(iter->second);
		}
		else if (iter->second->type == "DECISION") {
			decision_nodes.push_back(iter->second);
		}
	}

	for (map<int, edge*>::iterator iter = edges_map.begin(); iter != edges_map.end(); ++iter) {
		if (iter->second->type == "CHANCE") {
			chance_edges.push_back(iter->second);
		}else if (iter->second->type == "DECISION") {
			decision_edges.push_back(iter->second);
		}
	}



	ofstream save(file_name + "/tree_data.txt");



	save << "end_node" << " " << end_nodes.size() << endl;
	for (int i = 0; i <= end_nodes.size() - 1; i++) {
		save << end_nodes[i]->id << " " << end_nodes[i]->value << endl;
	}

	save << "chance_node" << " " << chance_nodes.size() << endl;
	for (int i = 0; i <= chance_nodes.size() - 1; i++) {
		save << chance_nodes[i]->id << endl;
		save << chance_nodes[i]->description << endl;
	}

	save << "decision_node" << " " << decision_nodes.size() << endl;
	for (int i = 0; i <= decision_nodes.size() - 1; i++) {
		if (decision_nodes[i]->is_root()) {
			save << decision_nodes[i]->id << " y" << endl;
		}
		else {
			save << decision_nodes[i]->id << " n" << endl;
		}
		save << decision_nodes[i]->description << endl;
	}


	save << "chance_edge" << " " << chance_edges.size() << endl;
	for (int i = 0; i <= chance_edges.size() - 1; i++) {
		save << chance_edges[i]->id << " " << chance_edges[i]->prev->id << " " << chance_edges[i]->next->id << " " << chance_edges[i]->get_probability() << endl;
		save << chance_edges[i]->description << endl;
	}


	save << "decision_edge" << " " << decision_edges.size() << endl;
	for (int i = 0; i <= decision_edges.size() - 1; i++) {
		save << decision_edges[i]->id << " " << decision_edges[i]->prev->id << " " << decision_edges[i]->next->id << " " << decision_edges[i]->get_costs() << endl;
		save << decision_edges[i]->description << endl;
	}
	save.close();

}

void bayes_graph_engine::generate_report(map<int, edge*>& edges_map, map<int, node*>& nodes_map, vector<tree_element*>& tree, string file_name)
{
	tree_element* root = find_root(tree);
	vector < vector<tree_element*>> winners;
	for (map<int, node*>::iterator iter = nodes_map.begin(); iter != nodes_map.end(); ++iter) {
		if (iter->second->type == "END" && iter->second->get_winner()) {
			vector<tree_element* > winner;

			tree_element* element = iter->second;

			while (element->get_prev() != NULL) {
				winner.push_back(element);
				element = element->get_prev();
			}winner.push_back(element);
			winners.push_back(winner);
		}
	}
	ofstream decision(file_name + "/najlepsza_decyzja_raport.txt");
	for (int i = 0; i <= winners.size() - 1; i++) {
		int number = 1;
		decision << "Najlepsza możliwa decyzja:" << endl;
		for (int j = winners[i].size() - 1; j >= 0; j--) {
			decision << number << ". (id: " << winners[i][j]->id << ") " << winners[i][j]->get_type() << " opis: " << winners[i][j]->description << "  wartość: " << winners[i][j]->get_value() << endl;
			number++;
		}decision << endl;
	}
	decision << endl;
	decision.close();

	ofstream calculations(file_name + "/etapy_obliczen_raport.txt");

	calculations << "Wyniki poszczególnych etapów obliczeń:" << endl;

	for (map<int, node*>::iterator iter = nodes_map.begin(); iter != nodes_map.end(); ++iter) {
		calculations << iter->second->get_type() << " (id: " << iter->second->id << ") opis: " << iter->second->description << " wartość: " << iter->second->value << endl;
	}calculations << endl;
	for (map<int, edge*>::iterator iter = edges_map.begin(); iter != edges_map.end(); ++iter) {
		if (iter->second->type == "DECISION") {
			calculations << iter->second->get_type() << " koszt: " << iter->second->get_costs() << " (id: " << iter->second->id << ") opis: " << iter->second->description << " wartość: " << iter->second->get_value() << endl;
		}
		else {
			calculations << iter->second->get_type() << " prawdopodobieństwo: " << iter->second->get_probability() << " (id: " << iter->second->id << ") opis: " << iter->second->description << " wartość: " << iter->second->get_value() << endl;
		}
	}
	calculations.close();
}
