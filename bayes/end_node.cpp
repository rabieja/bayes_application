#include "end_node.h"

end_node::end_node(int id, double value)
{
	this->id = id;
	this->value = value;
	this->next = next;
	this->type = "END";
	this->description = to_string(value);
}

bool end_node::is_node()
{
	return true;
}

bool end_node::is_edge()
{
	return false;
}

bool end_node::is_root()
{
	return false;
}
