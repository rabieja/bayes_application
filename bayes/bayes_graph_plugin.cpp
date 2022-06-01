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
#include <io.h>


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
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);
	double number;

	cin >> number;
	logs << number << endl;
	while (1)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits < streamsize >::max(), '\n');
			cout << "Podane dane nie s� prawid�owe. Spr�buj ponownie." << endl;
			logs << "Podane dane nie s� prawid�owe. Spr�buj ponownie." << endl;
			cin >> number;
			logs << number << endl;
		}
		if (!cin.fail()) break;
	}
	cin.ignore(numeric_limits < streamsize >::max(), '\n');
	return number;
}
int bayes_graph_plugin::get_int_number() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	int number;
	cin >> number;
	logs << number << endl;
	while (1)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits < streamsize >::max(), '\n');
			cout << "Podane dane nie s� prawid�owe. Spr�buj ponownie." << endl;
			logs << "Podane dane nie s� prawid�owe. Spr�buj ponownie." << endl;

			cin >> number;
			logs << number << endl;

		}
		if (!cin.fail()) break;
	}
	cin.ignore(numeric_limits < streamsize >::max(), '\n');
	return number;
}
void bayes_graph_plugin::add_chance_edge(node* prev, node* next, string description) {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	id++;
	double probability = 2;
	while (probability > 1 || probability < 0) {
		cout << "Podaj prawdopodobie�stwo (warto�� z przedzia�u <0, 1>)..." << endl;
		logs << "Podaj prawdopodobie�stwo (warto�� z przedzia�u <0, 1>)..." << endl;

		probability = get_number();
	}
	edge* edge_element = new chance_edge(id, prev, next, probability, description);

	next->add_prev_element(edge_element);
	prev->add_next_element(edge_element);
	edges_map.insert(pair<int, edge*>(id, edge_element));
	tree.push_back(edge_element);
}

void bayes_graph_plugin::add_decision_edge(node* prev, node* next, string description) {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	id++;
	double cost;
	cout << "Podaj koszty uzyskania dodatkowych informacji, je�li nie istniej� podaj \"0\"..." << endl;
	logs << "Podaj koszty uzyskania dodatkowych informacji, je�li nie istniej� podaj \"0\"..." << endl;

	cost = get_number();
	edge* edge_element = new decision_edge(id, prev, next, cost, description);

	next->add_prev_element(edge_element);
	prev->add_next_element(edge_element);
	edges_map.insert(pair<int, edge*>(id, edge_element));
	tree.push_back(edge_element);
}

void bayes_graph_plugin::add_edge(int prev_id, int next_id) {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string description;
	cout << "Podaj opis kraw�dzi..." << endl;
	logs << "Podaj opis kraw�dzi..." << endl;
	//cin.ignore(numeric_limits < streamsize >::max(), '\n');
	getline(cin, description);
	logs << description << endl;

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
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	if (nodes.count(id) == 0) {
		cout << "Podany w�ze� nie istnieje lub posiada ju� poprzednika. Spr�buj ponownie." << endl;
		logs << "Podany w�ze� nie istnieje lub posiada ju� poprzednika. Spr�buj ponownie." << endl;

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
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	cout << endl;
	cout << "LISTA DOST�PNYCH WIERZCHO�K�W" << endl;
	cout << endl;
	logs << endl;
	logs << "LISTA DOST�PNYCH WIERZCHO�K�W" << endl;
	logs << endl;

	string type;
	for (map<int, node*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter) {
		cout << "id:" << iter->first << "  opis: " << iter->second->description << "  typ: " << iter->second->get_type() << endl;
		logs << "id:" << iter->first << "  opis: " << iter->second->description << "  typ: " << iter->second->get_type() << endl;

	}
	cout << endl;
	logs << endl;

}
void bayes_graph_plugin::generate_graph() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string answer, file_name;
	if (engine.validation(tree, edges_map, nodes_map)) {
		engine.find_decision(tree);

		string str;
		cout << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� drzewo decyzyjne" << endl;
		logs << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� drzewo decyzyjne" << endl;

		getline(cin, str);
		logs << str << endl;
		char* data_file = const_cast<char*>(str.c_str());
		while (_access(data_file, 00) != 0) {
			if (_access(data_file, 00) == 0) {
				break;
			}

			cout << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� drzewo decyzyjne" << endl;
			logs << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� drzewo decyzyjne" << endl;
			getline(cin, str);
			logs << str << endl;
			char* data_file = const_cast<char*>(str.c_str());
		}

		engine.create_dot_graph(data_file, tree, edges_map, nodes_map);
		engine.create_png_graph(data_file);
		cout << "Generowanie drzewa decyzyjnego przebieg�o pomy�lnie." << endl;
		logs << "Generowanie drzewa decyzyjnego przebieg�o pomy�lnie." << endl;

	}
	else {
		cout << "Nie uda�o si� wygenerowa� drzewa decyzyjnego. Program zako�czy prace." << endl;
		logs << "Nie uda�o si� wygenerowa� drzewa decyzyjnego. Program zako�czy prace." << endl;

		exit(0);
	}
}
void bayes_graph_plugin::generate_decision_node(bool root) {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string description;
	id++;
	cout << "Podaj opis w�z�a" << endl;
	logs << "Podaj opis w�z�a" << endl;
	getline(cin, description);
	logs << description << endl;
	node* node_element = new decision_node(id, root, description);

	nodes_map.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);
}

void bayes_graph_plugin::generate_chance_node() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string description;
	id++;
	cout << "Podaj opis w�z�a" << endl;
	logs << "Podaj opis w�z�a" << endl;
	getline(cin, description);
	logs << description << endl;

	node* node_element = new chance_node(id, description);
	nodes_map.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);
}

void bayes_graph_plugin::generate_end_node() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	double value;
	id++;
	cout << "Podaj warto�� w�z�a" << endl;
	logs << "Podaj warto�� w�z�a" << endl;
	value = get_number();

	node* node_element = new end_node(id, value);
	nodes_map.insert(pair<int, node*>(id, node_element));
	tree.push_back(node_element);
}

void bayes_graph_plugin::generate_tree_from_file() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);
	string data_file;

	cout << "Podaj �cie�k� do pliku z kt�rego chcesz wygenerowa� drzewo..." << endl;	
	logs << "Podaj �cie�k� do pliku z kt�rego chcesz wygenerowa� drzewo..." << endl;
	getline(cin, data_file);
	logs << data_file;
	while (!generate_from_file(data_file)) {
		cout << "Podany plik nie istnieje. Podaj ponownie �cie�k� do pliku" << endl;
		logs << "Podany plik nie istnieje. Podaj ponownie �cie�k� do pliku" << endl;

		getline(cin, data_file);
		logs << data_file;

	}
	generate_graph();
}

void bayes_graph_plugin::manual_generate_root() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	cout << "Zacznij od w�z�a pocz�tkowego - korzenia." << endl;
	logs << "Zacznij od w�z�a pocz�tkowego - korzenia." << endl;
	generate_decision_node(true);
}
void bayes_graph_plugin::manual_generate_decision_nodes() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string n_decision = "";

	while (n_decision != "nie" && n_decision != "NIE") {
		cout << "Czy chcesz doda� kolejny w�ze� decyzyjny?" << endl;
		logs << "Czy chcesz doda� kolejny w�ze� decyzyjny?" << endl;

		getline(cin, n_decision);
		logs << n_decision << endl;
		if (n_decision == "tak" || n_decision == "TAK") {
			generate_decision_node(false);
		}
	}
}
void bayes_graph_plugin::manual_generate_chance_nodes() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string n_chance = "";
	cout << "Dodaj w�z�y losowe..." << endl;
	logs << "Dodaj w�z�y losowe..." << endl;

	while (n_chance != "tak" && n_chance != "nie" && n_chance != "TAK" && n_chance != "NIE") {
		cout << "Czy chcesz doda� w�ze� losowy?" << endl;
		logs << "Czy chcesz doda� w�ze� losowy?" << endl;
		getline(cin, n_chance);
		logs << n_chance << endl;
		if (n_chance == "tak" || n_chance == "TAK") {
			generate_chance_node();
		}
	}
	if (n_chance != "nie" && n_chance != "NIE") {
		n_chance = "";
	}

	while (n_chance != "nie" && n_chance != "NIE") {
		cout << "Czy chcesz doda� kolejny w�ze� losowy?" << endl;
		logs << "Czy chcesz doda� kolejny w�ze� losowy?" << endl;

		getline(cin, n_chance);
		logs << n_chance << endl;

		if (n_chance == "tak" || n_chance == "TAK") {
			generate_chance_node();
		}
	}

}

void bayes_graph_plugin::manual_generate_end_nodes() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string n_end = "";
	cout << "Dodaj w�z�y ko�cowe..." << endl;
	logs << "Dodaj w�z�y ko�cowe..." << endl;

	while (n_end != "tak" && n_end != "nie" && n_end != "TAK" && n_end != "NIE") {
		cout << "Czy chcesz doda� w�ze� ko�cowy?" << endl;
		logs << "Czy chcesz doda� w�ze� ko�cowy?" << endl;

		getline(cin, n_end);
		logs << n_end << endl;
		if (n_end == "tak" || n_end == "TAK") {
			generate_end_node();
		}
	}
	if (n_end != "nie" && n_end != "NIE") {
		n_end = "";
	}
	while (n_end != "nie" && n_end != "NIE") {
		cout << "Czy chcesz doda� kolejny w�ze� ko�cowy?" << endl;
		logs << "Czy chcesz doda� kolejny w�ze� ko�cowy?" << endl;

		getline(cin, n_end);
		logs << n_end << endl;

		if (n_end == "tak" || n_end == "TAK") {
			generate_end_node();
		}
	}
}

void bayes_graph_plugin::manuale_generate_edges() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string e_change = "", e_decision = "", edge_answer = "";

	map <int, node*> helper_nodes_map;
	vector <node*> helper_nodes;
	helper_nodes_map = nodes_map;
	int next_id;

	erase_node(helper_nodes_map, helper_nodes, 1);

	while (!helper_nodes.empty() && !helper_nodes_map.empty()) {
		string type_description;

		cout << "Podaj id w�z�a z kt�rym chcesz po��czy� w�ze�: id: " << helper_nodes[0]->id << " nazwa: " << helper_nodes[0]->description << " typ: " << helper_nodes[0]->get_type() << endl;
		logs << "Podaj id w�z�a z kt�rym chcesz po��czy� w�ze�: id: " << helper_nodes[0]->id << " nazwa: " << helper_nodes[0]->description << " typ: " << helper_nodes[0]->get_type() << endl;

		show_all_nodes(helper_nodes_map);
		while (next_id = get_int_number()) {
			if (erase_node(helper_nodes_map, helper_nodes, next_id)) {
				break;
			}
		}
		add_edge(helper_nodes[0]->id, next_id);

		while ((edge_answer != "nie" || edge_answer != "NIE" ) && !helper_nodes_map.empty()) {
			while (edge_answer != "nie" && edge_answer != "tak" && edge_answer != "NIE" && edge_answer != "TAK") {
				cout << "Czy chcesz doda� kolejne po��czenie do tego w�z�a?" << endl;
				logs << "Czy chcesz doda� kolejne po��czenie do tego w�z�a?" << endl;
				getline(cin, edge_answer);
				logs << edge_answer << endl;
			}
			if (edge_answer == "nie" || edge_answer == "NIE") {
				edge_answer = "";
				break;
			}
			cout << "Dost�pne w�z�y:" << endl;
			logs << "Dost�pne w�z�y:" << endl;
			show_all_nodes(helper_nodes_map);
			cout << "Podaj id w�z�a z kt�rym chcesz po��czy� ten w�ze�" << endl;
			logs << "Podaj id w�z�a z kt�rym chcesz po��czy� ten w�ze�" << endl;

			while (next_id = get_int_number()) {
				if (erase_node(helper_nodes_map, helper_nodes, next_id)) {
					break;
				}
			}
			add_edge(helper_nodes[0]->id, next_id);
			edge_answer = "";

		}
		helper_nodes.erase(helper_nodes.begin());
	}
}
void bayes_graph_plugin::manual_generation_tree() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string n_end = "",  e_change = "", e_decision = "";
	cout << "Zosta� uruchomiony tryb r�cznego generowania danych." << endl;
	cout << "Dodaj w�z�y decyzyjne..." << endl;
	cout << endl;

	logs << "Zosta� uruchomiony tryb r�cznego generowania danych." << endl;
	logs << "Dodaj w�z�y decyzyjne..." << endl;
	logs << endl;
	manual_generate_root();

	manual_generate_decision_nodes();
	manual_generate_chance_nodes();
	manual_generate_end_nodes();

	cout << "Po��cz ze sob� w�z�y..." << endl;
	cout << endl;
	cout << "Aby wybra� w�ze� z kt�rym po��czysz podany przez aplikacje w�ze� wpisz jego id." << endl;
	cout << "Na pocz�tek po��cz korze� drzewa z kolejnymi w�z�ami" << endl;
	cout << endl;
	cout << "Poni�ej znajduje si� lista w�z��w, kt�re nie posiadaj� jeszcze poprzednika." << endl;
	cout << endl;

	logs << "Po��cz ze sob� w�z�y..." << endl;
	logs << endl;
	logs << "Aby wybra� w�ze� z kt�rym po��czysz podany przez aplikacje w�ze� wpisz jego id." << endl;
	logs << "Na pocz�tek po��cz korze� drzewa z kolejnymi w�z�ami" << endl;
	logs << endl;
	logs << "Poni�ej znajduje si� lista w�z��w, kt�re nie posiadaj� jeszcze poprzednika." << endl;
	logs << endl;

	manuale_generate_edges();

	generate_graph();
}


void bayes_graph_plugin::save_graph_to_file() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string str;
	cout << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� dane" << endl;
	logs << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� dane" << endl;

	getline(cin, str);
	logs << str << endl;
	char* data_file = const_cast<char*>(str.c_str());
	while (_access(data_file, 00) != 0) {
		if (_access(data_file, 00) == 0) {
			break;
		}
		cout << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� dane" << endl;
		logs << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� dane" << endl;
		getline(cin, str);
		logs << str << endl;
		char* data_file = const_cast<char*>(str.c_str());
	}
	engine.save_graph_to_file(edges_map, nodes_map, tree, data_file);
	cout << "Zapis danych powi�d� si�." << endl;
	logs << "Zapis danych powi�d� si�." << endl;
}

void bayes_graph_plugin::generate_report() {
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt", ios::out | ios::app);

	string str;
	cout << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� raporty z wynikami" << endl;
	logs << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� raporty z wynikami" << endl;

	getline(cin, str);
	logs << str << endl;
	char* data_file = const_cast<char*>(str.c_str());
	while (_access(data_file, 00) != 0) {
		if (_access(data_file, 00) == 0) {
			break;
		}
		cout << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� raporty z wynikami" << endl;
		logs << "Podaj �cie�k� do folderu w kt�rym chcesz zapisa� raporty z wynikami" << endl;

		getline(cin, str);
		logs << str << endl;
		char* data_file = const_cast<char*>(str.c_str());
	}
	engine.generate_report(edges_map, nodes_map, tree, data_file);
	cout << "Generowanie raport�w powiod�o si�." << endl;
	logs << "Generowanie raport�w powiod�o si�." << endl;
}
void bayes_graph_plugin::run()
{
	ofstream logs("C:\\Users\\Agniesia\\Desktop\\1\\logs.txt");
	string answer= "";
	cout << "Witaj w programie wspomagaj�cym podejmowanie decyzji z wykorzystaniem metody Bayesa." << endl;
	logs << "Witaj w programie wspomagaj�cym podejmowanie decyzji z wykorzystaniem metody Bayesa." << endl;
	cout << endl;
	logs << endl;
	while (answer != "tak" && answer != "nie" && answer != "TAK" && answer != "NIE") {
		cout << "Czy chcesz wygenerowa� drzewo decyzyjne z danych zapisanych w pliku? (tak/nie)" << endl;
		logs << "Czy chcesz wygenerowa� drzewo decyzyjne z danych zapisanych w pliku? (tak/nie)" << endl;

		getline(cin, answer);
		logs << answer;

		if (answer == "tak" || answer == "TAK") {
			generate_tree_from_file();
			generate_report();
		}
		else if (answer == "nie" || answer == "NIE") {
			manual_generation_tree();
			generate_report();
			save_graph_to_file();
		}
	}

	logs.close();
}
