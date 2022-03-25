#include "edge.h"
#include <iostream>

bool edge::is_root()
{
    return false;
}

void edge::set_value()
{
    this->next->set_value();
    cout << "ccc";
    return;
}
