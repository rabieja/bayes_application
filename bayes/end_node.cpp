#include "end_node.h"

end_node::end_node(int id, long double value)
{
	this->id = id;
	this->value = value;
	this->next = next;
	this->type = "END";
	this->description = to_string(value);
	this->helper = false;
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

void end_node::find_winner()
{
	return;
}

void end_node::set_helper(bool helper_value)
{
	this->helper = helper_value;
}

string end_node::get_type()
{
	return "Wêze³ koñcowy";
}
