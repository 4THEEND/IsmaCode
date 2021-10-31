#pragma once

#include <iostream>
#include <vector>

#include "Logging.h"
#include "Expression.h"

struct Node {
	std::string type;
	Expression expression;
	std::shared_ptr<Node> left = nullptr;
	std::shared_ptr<Node> right = nullptr;
};

struct AppendResult {
	bool isGood;
	std::shared_ptr<Node> lastNode;
};

enum class Side
{
	Right,
	Left
};

class IsmTree
{
public:
	IsmTree(Node node = { ENTRY_POINT, { {"", ""} } });
	AppendResult append(Node node_insered, std::shared_ptr<Node> insered_to, Side side = Side::Left);
	std::shared_ptr<Node> clear();
	inline std::shared_ptr<Node> getHead() { return m_nodes[0]; }
#ifdef _DEBUG
	void print();
#endif 

private:
	std::vector<std::shared_ptr<Node>> m_nodes = {};
	unsigned int m_lastIndex = 0;

};

