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
#include <locale.h>

using namespace std;
int id = 0;

bayes_graph_plugin::bayes_graph_plugin() {
	map <int, edge*> edges_map;
	map <int, node*> nodes_map;
	vector <tree_element*> tree;

	bayes_graph_engine bayes_graph_engine;
	this->engine = bayes_graph_engine;

	this->edges_map = edges_map;
	this->nodes_map = nodes_map;
	this->tree = tree;
}

bool bayes_graph_plugin::generate_from_file(string file_name) {
	string type = "", chance_node_description, decision_node_description, edge_description;
	char root;
	double end_node_value, cost_of_additional_information, probability;
	int size, id, prev_tree_element, next_tree_element;

	fstream data_file;
	data_file.open(file_name);
	if (!data_file) {
		return false;
	}
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
				nodes_map.insert(pair<int, node*>(id, node_element));
				tree.push_back(node_element);
			}continue;
		}
		else if (type == "chance_node") {
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

	return true;
}
double bayes_graph_plugin::get_number() {
	double number;
	cin >> number;
	while (1)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits < streamsize >::max(), '\n');
			cout << "Podane dane nie s� prawid�owe. Spr�buj ponownie." << endl;
			cin >> number;
		}
		if (!cin.fail()) break;
	}
	return number;
}
void bayes_graph_plugin::add_chance_edge(node* prev, node* next, string description) {
	id++;
	double probability = 2;
	while (probability > 1 || probability < 0) {
		cout << "Podaj prawdopodobie�stwo (warto�� z przedzia�u <0, 1>)..." << endl;
		probability = get_number();
	}
	edge* edge_element = new chance_edge(id, prev, next, probability, description);

	next->add_prev_element(edge_element);
	prev->add_next_element(edge_element);
	edges_map.insert(pair<int, edge*>(id, edge_element));
	tree.push_back(edge_element);
}

void bayes_graph_plugin::add_decision_edge(node* prev, node* next, string description) {
	id++;
	double cost;
	cout << "Podaj koszty uzyskania dodatkowych informacji, je�li nie istniej� podaj \"0\"..." << endl;
	cost = get_number();
	edge* edge_element = new decision_edge(id, prev, next, cost, description);

	next->add_prev_element(edge_element);
	prev->add_next_element(edge_element);
	edges_map.insert(pair<int, edge*>(id, edge_element));
	tree.push_back(edge_element);
}

void bayes_graph_plugin::add_edge(int prev_id, int next_id, vector <node*>& helper_nodes) {
	string description;
	cout << "Podaj opis kraw�dzi..." << endl;
	cin.ignore(numeric_limits < streamsize >::max(), '\n');
	getline(cin, description);

	node* prev = nodes_map.find(prev_id)->second;
	node* next = nodes_map.find(next_id)->second;

	if (prev->type == "DECISION") {
		add_decision_edge(prev, next, description);
	}
	else if (prev->type == "CHANCE") {
		add_chance_edge(prev, next, description);
	}
}
bool bayes_graph_plugin::erase_node(map <int, node*>& nodes, vector <node*>& helper_nodes, int id) {
	if (nodes.count(id) == 0) {
		cout << "Podany w�ze� nie istnieje lub posiada ju� poprzednika. Spr�buj ponownie." << endl;
		return false;
	}
	else {
		if (nodes.find(id)->second->type != "END") {
			helper_nodes.push_back(nodes.find(id)->second);
		}
		nodes.erase(id);
		return true;
	}
}
void bayes_graph_plugin::show_all_nodes(map <int, node*>& nodes) {
	cout << endl;
	cout << "LISTA DOST�PNYCH WIERZCHO�K�W" << endl;
	cout << endl;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		cout << "id:" << iter->first << "  opis: " << iter->second->description << "  typ: " << iter->second->type << endl;
	}
}
void bayes_graph_plugin::generate_graph() {
	string answer;
	if (engine.validation(tree, edges_map, nodes_map)) {
		engine.find_decision(tree);

		string file_name = "dane";

		engine.create_dot_graph(file_name, tree, edges_map, nodes_map);
		engine.create_png_graph(file_name);
		cout << "Generowanie drzewa decyzyjnego przebieg�o pomy�lnie." << endl;
	}
	else {
		cout << "Nie uda�o si� wygenerowa� drzewa decyzyjnego. Czy chcesz zako�czy� prac� programu? (tak/nie)" << endl;
		cin >> answer;
		if (answer == "tak") {
			exit(0);
		}
	}
}
void bayes_graph_plugin::generate_decision_node(bool root) {
	string description;
	id++;
	cout << "Podaj opis w�z�a" << endl;
	cin.ignore(numeric_limits < streamsize >::max(), '\n');
	getline(cin, description);

	node* node_element = new decision_node(id, root, description);

	nodes_map.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);
}

void bayes_graph_plugin::generate_chance_node() {
	string description;
	id++;
	cout << "Podaj opis w�z�a" << endl;
	cin.ignore(numeric_limits < streamsize >::max(), '\n');
	getline(cin, description);

	node* node_element = new chance_node(id, description);
	nodes_map.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);
}

void bayes_graph_plugin::generate_end_node() {
	double value;
	id++;
	cout << "Podaj warto�� w�z�a" << endl;
	value = get_number();

	node* node_element = new end_node(id, value);
	nodes_map.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);
}


void bayes_graph_plugin::run()
{
	setlocale(LC_CTYPE, "Polish");
	string answer= "", data_file;
	cout << "Witaj w programie wspomagaj�cym podejmowanie decyzji z wykorzystaniem metody Bayesa." << endl;
	while (answer != "tak" && answer != "nie") {
		cout << "Czy chcesz wygenerowa� drzewo decyzyjne z danych zapisanych w pliku? (tak/nie)" << endl;
		cin >> answer;
		if (answer == "tak") {
			cout << "Podaj �cie�k� do pliku z kt�rego chcesz wygenerowa� drzewo..." << endl;
			cin >> data_file;
			while (!generate_from_file(data_file)) {
				cout << "Podany plik nie istnieje. Podaj ponownie �cie�k� do pliku" << endl;
				cin >> data_file;
			}
			generate_graph();
		}
		else if (answer == "nie") {
			string n_end = "", n_chance = "", n_decision = "", e_change = "", e_decision = "";
			cout << "Zosta� uruchomiony tryb r�cznego generowania danych." << endl;
			cout << "Dodaj w�z�y decyzyjne..." << endl;
			cout << "Zacznij od w�z�a pocz�tkowego - korzenia." << endl;
			generate_decision_node(true);

			while (n_decision != "nie") {
				cout << "Czy chcesz doda� kolejny w�ze� decyzyjny?" << endl;
				cin >> n_decision;
				if (n_decision == "tak") {
					generate_decision_node(false);
				}
			}

			cout << "Dodaj w�z�y losowe..." << endl;
	
			while (n_chance != "tak" && n_chance != "nie") {
				cout << "Czy chcesz doda� w�ze� losowy?" << endl;
				cin >> n_chance;
				if (n_chance == "tak") {
					generate_chance_node();
				}
			}
			if (n_chance != "nie") {
				n_chance = "";
			}

			while (n_chance != "nie") {
				cout << "Czy chcesz doda� kolejny w�ze� losowy?" << endl;
				cin >> n_chance;
				if (n_chance == "tak") {
					generate_chance_node();
				}
			}

			cout << "Dodaj w�z�y ko�cowe..." << endl;

			while (n_end != "tak" && n_end != "nie") {
				cout << "Czy chcesz doda� w�ze� ko�cowy?" << endl;
				cin >> n_end;
				if (n_end == "tak") {
					generate_end_node();
				}
			}
			if (n_end != "nie") {
				n_end = "";
			}
			while (n_end != "nie") {
				cout << "Czy chcesz doda� kolejny w�ze� ko�cowy?" << endl;
				cin >> n_end;
				if (n_end == "tak") {
					generate_end_node();
				}
			}

	
			map <int, node*> helper_nodes_map;
			vector <node*> helper_nodes;
			helper_nodes_map = nodes_map;

			cout << "Po��cz ze sob� w�z�y..." << endl;
			cout << endl;
			cout << "Aby wybra� w�ze� z kt�rym po��czysz podany przez aplikacje w�ze� wpisz jego id." << endl;
			cout << "Na pocz�tek po��cz korze� drzewa z kolejnymi w�z�ami" << endl;
			cout << endl;
			cout << "Poni�ej znajduje si� lista w�z��w, kt�re nie posiadaj� jeszcze poprzednika." << endl;
			cout << endl;
			erase_node(helper_nodes_map, helper_nodes, 1);

			int next_id;
			string edge_answer = "";
			while (!helper_nodes.empty()) {

				cout << "Podaj id w�z�a z kt�rym chcesz po��czy� w�ze�: id: " << helper_nodes[0]->id << "   nazwa: " << helper_nodes[0]->description << endl;
				show_all_nodes(helper_nodes_map);
				while (next_id = get_number()) {
					if (erase_node(helper_nodes_map, helper_nodes, next_id)) {
						break;
					}
				}
				add_edge(helper_nodes[0]->id, next_id, helper_nodes);
			
				while (edge_answer != "nie" && !helper_nodes_map.empty()) {
					while (edge_answer != "nie" && edge_answer != "tak") {
						cout << "Czy chcesz doda� kolejne po��czenie do tego w�z�a?" << endl;
						cin >> edge_answer;
					}
					if (edge_answer == "nie") {
						break;
					}
					cout << "Dost�pne w�z�y:" << endl;
					show_all_nodes(helper_nodes_map);
					cout << "Podaj id w�z�a z kt�rym chcesz po��czy� ten w�ze�" << endl;
					while (next_id = get_number()) {
						if (erase_node(helper_nodes_map, helper_nodes, next_id)) {
							break;
						}
					}
					add_edge(helper_nodes[0]->id, next_id, helper_nodes);
					cout << "Czy chcesz doda� kolejne po��czenie do tego w�z�a?" << endl;
					cin >> edge_answer;
				}
				helper_nodes.erase(helper_nodes.begin());
			}
			generate_graph();
		}
	}

}
