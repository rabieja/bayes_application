#include "bayes_graph_engine.h"
#include "node.h"
#include "edge.h"
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

void bayes_graph_engine::validation(map<int, edge*> edges, map<int, node*> nodes)
{
	setlocale(LC_CTYPE, "Polish");
	validate_nodes(nodes);
	validate_edges(edges);
}

void bayes_graph_engine::validate_nodes(map<int, node*> nodes)
{
	string description = "";
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		description = iter->second->description;
		if (!iter->second->is_root() && iter->second->prev == NULL) {
			cout << "Wprowadzone dane sa nieprawid�owe, w�ze� \" " << description << " \" nie posiada poprzednika." << endl;
		}
	}
}

void bayes_graph_engine::validate_edges(map<int, edge*> edges)
{
	string description = "";
	for (map<int, edge*>::iterator iter = edges.begin(); iter != edges.end(); ++iter) {
		description = iter->second->description;
		if (iter->second->prev == NULL) {
			cout << "Wprowadzone dane sa nieprawid�owe, ga��� \" " << description << " \" nie posiada poprzednika." << endl;
		}
		if (iter->second->next == NULL) {
			cout << "Wprowadzone dane sa nieprawid�owe, ga��� \" " << description << " \" nie posiada nast�pnika." << endl;
		}
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

