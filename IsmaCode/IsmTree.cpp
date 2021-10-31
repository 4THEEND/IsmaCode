#include "IsmTree.h"

IsmTree::IsmTree(Node node)
{
	m_nodes = { std::shared_ptr<Node>(new Node(node)) };
}

AppendResult IsmTree::append(Node node_insered, std::shared_ptr<Node> insered_to, Side side)
{
	std::shared_ptr<Node> node_temp(new Node(node_insered));
	m_nodes.push_back(node_temp);
	for (std::shared_ptr<Node> node : m_nodes)
	{
		if (node == insered_to)
		{
			if (side == Side::Left)
				node->left = node_temp;
			else
				node->right = node_temp;
			return { true, node_temp };
		}
	}
	return { false, nullptr };
}

std::shared_ptr<Node> IsmTree::clear()
{
	m_nodes = { std::shared_ptr<Node>(new Node({ ENTRY_POINT, { {"", ""} } })) };
	return m_nodes[0];
}

#ifdef _DEBUG
void IsmTree::print()
{
	//TODO make that
}
#endif 
