#include "edge.h"
#include <iostream>

bool edge::is_root()
{
    return false;
}

void edge::set_value()
{
    this->next->set_value();
    return;
}

void edge::find_winner()
{
    this->next->set_winner();
    this->next->find_winner();
}
