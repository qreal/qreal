#include "dirtree.h"

DirTree::DirTree(QString name, int localTesting, int localDocumented, int localTests, bool isIgnored, bgcolors color)
{
	root = DirTree::createNode(name, localTesting, localDocumented, localTests, isIgnored, color);
}

void DirTree::addChild(DirNode *child, DirNode *parent)
{
	if (!parent->childNode) {
		parent->childNode = child;
	} else {
		DirNode *node = parent->childNode;
		while (node->rightNode != NULL) {
			node = node->rightNode;
		}
		node->rightNode = child;
	}
	child->parentNode = parent;
}

DirNode * DirTree::getRoot() const
{
	return root;
}


DirTree::~DirTree()
{
	delNode(root);
}

void DirTree::delNode(DirNode *node)
{
	if (node->childNode) {
		delNode(node->childNode);
	}
	if (node->rightNode){
		delNode(node->rightNode);
	}
	delete node;
}

DirNode *DirTree::createNode(QString name, int localTesting, int localDocumented, int localTests, bool isIgnored, bgcolors color)
{
	DirNode *node = new DirNode;
	node->name = name;
	node->localDocumented = localDocumented;
	node->localTesting = localTesting;
	node->parentNode = NULL;
	node->rightNode = NULL;
	node->childNode = NULL;
	node->totalTesting = 0;
	node->totalDocumented = 0;
	node->totalTests = 0;
	node->localTests = localTests;
	node->isIgnored = isIgnored;
	node->color = color;
	return node;
}

void DirTree::calculateTotalData()
{
	calculateTotalDataForNode(root);
}

void DirTree::calculateTotalDataForNode(DirNode *node)
{
	if (node->childNode) {
		calculateTotalDataForNode(node->childNode);
	}
	if (node->rightNode) {
		calculateTotalDataForNode(node->rightNode);
	}

	node->totalDocumented += node->localDocumented;
	node->totalTesting += node->localTesting;
	node->totalTests += node->localTests;

	if (node != root) {
		node->parentNode->totalDocumented += node->totalDocumented;
		node->parentNode->totalTesting += node->totalTesting;
		node->parentNode->totalTests += node->totalTests;
	}
}
