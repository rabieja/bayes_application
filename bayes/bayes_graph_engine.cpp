#include "bayes_graph_engine.h"
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
	setlocale(LC_CTYPE, "Polish");
	if (!validate_nodes(tree, nodes, edges) || !validate_edges(edges)) {
		return false;
	}

	return true;
}

bool bayes_graph_engine::validate_nodes(vector<tree_element*>& tree, map<int, node*>& nodes, map<int, edge*>& edges)
{
	return validate_prev_element(nodes) && validate_next_elements(nodes) && validate_sum_probability(tree, nodes, edges);
}

bool bayes_graph_engine::validate_next_elements(map<int, node*>& nodes) {

	string description = "";
	bool result = true;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		description = iter->second->description;
		if (iter->second->type != "END" && iter->second->next.empty()) {
			cout << "Wprowadzone dane sa nieprawid³owe, wêze³ \" " << description << " \" nie posiada nastêpników." << endl;
			result = false;
		}
	}
	return result;
}

bool bayes_graph_engine::validate_prev_element(map<int, node*>& nodes) {

	string description = "";
	bool result = true;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		description = iter->second->description;
		if (!iter->second->is_root() && iter->second->prev == NULL) {
			cout << "Wprowadzone dane sa nieprawid³owe, wêze³ \" " << description << " \" nie posiada poprzednika." << endl;
			result = false;
		}
	}
	return result;
}

bool bayes_graph_engine::validate_sum_probability(vector<tree_element*>& tree, map<int, node*>& nodes, map<int, edge*>& edges) {

	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		if (iter->second->type == "CHANCE") {

			double sum = 0;
			for (int i = 0; i <= iter->second->next.size() - 1; i++) {
				if (iter->second->next[i]->type != "CHANCE") {
					cout << "Wprowadzone dane sa nieprawid³owe, ga³¹Ÿ \" " << iter->second->next[i]->description << " \" jest z³ego typu. Wymagany typ to ga³¹Ÿ losowa." << endl;
					return false;
				}
				else sum += iter->second->next[i]->get_probability();
			}

			if (sum > 1) {
				cout << "Wprowadzone dane sa nieprawid³owe, suma prawdopodobieñstw w wêŸle \" " << iter->second->description << " \" jest wiêksza od 1.0." << endl;
			}
			else if (sum < 1) {
				string answer;
				cout << "Wprowadzone dane sa nieprawid³owe, suma prawdopodobieñstw w wêŸle \" " << iter->second->description << " \" jest mniejsza od 1.0." << endl;
				cout << "Czy chcesz zredukowaæ b³¹d dodaj¹c wêze³ pomocniczy, którego wartoœæ monetarna jest równa 0? (tak/nie)" << endl;
				cin >> answer;
				if (answer == "tak" || answer == "Tak" || answer == "TAK") {
					generate_helper_node(tree, nodes, edges, iter->second, 1-sum);
				}
				else return false;
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
void bayes_graph_engine::generate_helper_node(vector<tree_element*>& tree, map<int, node*>& nodes, map<int, edge*>& edges, node * node_prev, double probability) {

	int id = search_next_id(tree);

	node* node_element = new end_node(id, 0);
	nodes.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);

	id++;
	edge* edge_element = new chance_edge(id, node_prev, node_element, probability, "");
	edges.insert(pair<int, edge*>(id, edge_element));
	tree.push_back(edge_element);

	node_element->add_prev_element(edge_element);
	node_prev->add_next_element(edge_element);
}
bool bayes_graph_engine::validate_edges(map<int, edge*> edges)
{
	bool result = true;
	string description = "";
	for (map<int, edge*>::iterator iter = edges.begin(); iter != edges.end(); ++iter) {
		description = iter->second->description;
		if (iter->second->prev == NULL) {
			cout << "Wprowadzone dane sa nieprawid³owe, ga³¹Ÿ \" " << description << " \" nie posiada poprzednika." << endl;
			result = false;
		}
		if (iter->second->next == NULL) {
			cout << "Wprowadzone dane sa nieprawid³owe, ga³¹Ÿ \" " << description << " \" nie posiada nastêpnika." << endl;
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
	Agraph_t* g;
	FILE* fp;
	gvc = gvContext();
	fp = fopen((file_name + ".dot").c_str(), "r");
	g = agread(fp, 0);
	gvLayout(gvc, g, "dot");
	gvRenderFilename(gvc, g, "png", (file_name + ".png").c_str());

	gvFreeLayout(gvc, g);
	agclose(g);
	return gvFreeContext(gvc);
}

void bayes_graph_engine::create_dot_graph(string file_name, vector<tree_element*> tree, map<int, edge*> edges, map<int, node*> nodes)
{
	// dot -Tpng dane.dot -o dane.png
	ofstream save(file_name + ".dot");
	save << "strict graph{" << endl;
	save << "rankdir = LR; " << endl;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		if (iter->second->type == "DECISION") {
			save << iter->second->id << "[shape=square  label=\" " << iter->second->value << "\" ] " << endl;
		}
		else if (iter->second->type == "CHANCE") {
			save << iter->second->id << "[shape=circle label=\" " << iter->second->value << "\"] " << endl;
		}
		else {
			save << iter->second->id << "[shape=none label=\" " << iter->second->value << "\"] " << endl;
		}
		if (iter->second->winner) {
			save << "[color = red];" << endl;
		}
		else save << "[color = black];" << endl;
	}
	for (map<int, edge*>::iterator iter = edges.begin(); iter != edges.end(); ++iter) {
		//cout << edges[i]->prev->id << ' ' << edges[i]->next->id << endl;
		if (iter->second->type == "DECISION") {
			save << iter->second->prev->id << " -- " << iter->second->next->id << "[label=\"" << iter->second->description << "\"] ";

		}
		else if (iter->second->type == "CHANCE") {
			save << iter->second->prev->id << " -- " << iter->second->next->id << "[label=\"" << iter->second->description << " " <<
				iter->second->get_probability() << "\"] ";
		}
		if (iter->second->winner) {
			save << "[color = red];" << endl;
		}
		else save << "[color = black];" << endl;
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

	ofstream save(file_name + "/graph_data.txt");

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


	save << "edge_chance" << " " << chance_edges.size() << endl;
	for (int i = 0; i <= chance_edges.size() - 1; i++) {
		save << chance_edges[i]->id << " " << chance_edges[i]->prev->id << " " << chance_edges[i]->next->id << " " << chance_edges[i]->get_probability() << endl;
		save << chance_edges[i]->description << endl;
	}


	save << "edge_decision" << " " << decision_edges.size() << endl;
	for (int i = 0; i <= decision_edges.size() - 1; i++) {
		save << decision_edges[i]->id << " " << decision_edges[i]->prev->id << " " << decision_edges[i]->next->id << " " << decision_edges[i]->get_costs() << endl;
		save << decision_edges[i]->description << endl;
	}
	save.close();
}