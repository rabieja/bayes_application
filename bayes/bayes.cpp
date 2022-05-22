#include <iostream>
#include "bayes_graph_plugin.h"
#include "edge.h"
#include <string>
#include <Windows.h>
#include <cstdio>
#include <io.h>
#include <locale.h>


using namespace std;

int main()
{
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
    setlocale(LC_ALL, "pl_PL.1250");

    bayes_graph_plugin * bayes = new bayes_graph_plugin();
    bayes->run();
}
